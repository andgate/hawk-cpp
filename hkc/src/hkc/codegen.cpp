#include "hkc/parser/node.h"
#include "hkc/codegen.h"
#include "parser.hpp"

#include <llvm/Support/raw_ostream.h>

using namespace std;

/* Compile the AST into a module */
void CodeGen::build_module(NModule& root)
{
    std::cout << "Generating code...\n";
    
    /* Create the top level interpreter function to call as entry */
    vector<Type*> argTypes;
    FunctionType *ftype = FunctionType::get(Type::getVoidTy(getGlobalContext()), makeArrayRef(argTypes), false);
    mainFunction = Function::Create(ftype, GlobalValue::ExternalLinkage, "main", module);
    BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", mainFunction, 0);
    
    /* Push a new variable/block context */
    pushBlock(bblock);
    root.accept(*this); /* emit bytecode for the toplevel block */
    ReturnInst::Create(getGlobalContext(), bblock);
    popBlock();
}

void CodeGen::print_ir()
{
    std::cout << "Code is generated.\n";
    PassManager<Module> pm;
    
    pm.addPass(PrintModulePass(outs()));
    pm.run(*module);
}

void CodeGen::write_ir(const std::string& out_file)
{
    std::error_code ec(errno, std::generic_category());
    raw_fd_ostream file(out_file, ec, sys::fs::OpenFlags::F_RW);
    module->print(file, NULL);
    file.close();
}

/* Returns an LLVM type based on the identifier */
static Type *typeOf(nident type) 
{
    if (type->name.compare("int") == 0) {
        return Type::getInt64Ty(getGlobalContext());
    }
    else if (type->name.compare("double") == 0) {
        return Type::getDoubleTy(getGlobalContext());
    }
    
    return Type::getVoidTy(getGlobalContext());
}

/* -- Code Generation -- */

void CodeGen::visit(NExpression& n)
{
}

void CodeGen::visit(NStatement& n)
{
}

void CodeGen::visit(NModule& n)
{
    std::cout << "Generating module " << n.name << endl;
    n.block->accept(*this);
}

void CodeGen::visit(NBlock& n)
{
    StatementList::const_iterator it;
    Value* last = nullptr;
    for (it = n.statements.begin(); it != n.statements.end(); it++) {
        std::cout << "Generating code for " << typeid(**it).name() << endl;
        (**it).accept(*this);
        last = vvalue;
    }
    
    std::cout << "Creating block" << endl;
    vvalue = last;
}

void CodeGen::visit(NInteger& n)
{
    std::cout << "Creating integer: " << n.value << endl;
    vvalue = ConstantInt::get(Type::getInt64Ty(getGlobalContext()), n.value, true);
}

void CodeGen::visit(NDecimal& n)
{
    std::cout << "Creating double: " << n.value << endl;
    vvalue = ConstantFP::get(Type::getDoubleTy(getGlobalContext()), n.value);
}

void CodeGen::visit(NString& n)
{
    std::cout << "Creating string: " << n.value << endl;
    vvalue = ConstantFP::get(Type::getDoubleTy(getGlobalContext()), n.value);
}

void CodeGen::visit(NIdentifier& n)
{
    std::cout << "Creating identifier reference: " << n.name << endl;
    if (locals().find(n.name) == locals().end()) {
        std::cerr << "undeclared variable " << n.name << endl;
        vvalue = nullptr;
    }
    vvalue = new LoadInst(locals()[n.name], "", false, currentBlock());
}

static Value* loadIdent(CodeGen& gen, nident id)
{
    std::cout << "Creating identifier reference: " << id->name << endl;
    if (gen.locals().find(id->name) == gen.locals().end()) {
        std::cerr << "undeclared variable " << id->name << endl;
        return nullptr;
    }
    
    return new LoadInst(gen.locals()[id->name], "", false, gen.currentBlock());
}

void CodeGen::visit(NFunctionCall& n)
{
    Function *function = module->getFunction(n.id->name.c_str());
    if (function == nullptr) {
        std::cerr << "no such function " << n.id->name << endl;

        // maybe it's a variable?
        vvalue = loadIdent(*this, n.id);
        return;
    }
    
    std::vector<Value*> args;
    ExpressionList::const_iterator it;
    for (it = n.arguments.begin(); it != n.arguments.end(); it++) {
        (**it).accept(*this);
        args.push_back(vvalue);
    }
    
    CallInst *call = CallInst::Create(function, makeArrayRef(args), "", currentBlock());
    std::cout << "Creating method call: " << n.id->name << endl;
    
    vvalue = call;
}

void CodeGen::visit(NBinaryOperator& n)
{
    std::cout << "Creating binary operation " << n.op << endl;
    Instruction::BinaryOps instr;
    
    if(n.op == "+") instr = Instruction::Add;
    if(n.op == "-") instr = Instruction::Sub;
    if(n.op == "*") instr = Instruction::Mul;
    if(n.op == "/") instr = Instruction::SDiv;
    
    n.lhs->accept(*this);
    Value* lhs_val = vvalue;
    
    n.rhs->accept(*this);
    Value* rhs_val = vvalue;
    
    vvalue = BinaryOperator::Create(instr, lhs_val, rhs_val, "", currentBlock());
}

void CodeGen::visit(NAssignment& n)
{
    std::cout << "Creating assignment for " << n.lhs->name << endl;
    if (locals().find(n.lhs->name) == locals().end()) {
        std::cerr << "undeclared variable " << n.lhs->name << endl;
        vvalue = nullptr;
    }
    
    n.rhs->accept(*this);
    Value* rhs_val = vvalue;
    
    vvalue = new StoreInst(rhs_val, locals()[n.lhs->name], false, currentBlock());
}



void CodeGen::visit(NExpressionStatement& n)
{
    std::cout << "Generating code for " << typeid(n.expression).name() << endl;
    n.expression->accept(*this);
}

void CodeGen::visit(NStatementExpression& n)
{
    
}

void CodeGen::visit(NReturnStatement& n)
{
    std::cout << "Generating return code for " << typeid(n.expression).name() << endl;
    n.expression->accept(*this);
    setCurrentReturnValue(vvalue);
}

void CodeGen::visit(NVariableDeclaration& n)
{
    if(n.bindings->names.size() != 1 && n.bindings->type_sig.size() != 1) {
        std::cerr << "Incomplete variable declaration!" << std::endl;
        exit(1);
    }
    
    auto& id = n.bindings->names.front();
    auto& type = n.bindings->type_sig.front();
    std::cout << "Creating variable declaration " << id->name << ": " << type->name << endl;
    
    AllocaInst *alloc = new AllocaInst(typeOf(type), id->name.c_str(), currentBlock());
    locals()[id->name] = alloc;
    if (n.lhs != nullptr) {
        NAssignment assn(id, n.lhs);
        assn.accept(*this);
    }
    
    vvalue = alloc;
}

static void make_variable(CodeGen& gen, nident id, nident type, nexpr lhs = nullptr)
{
    std::cout << "Creating variable declaration " << type->name << " " << id->name << endl;
    AllocaInst *alloc = new AllocaInst(typeOf(type), id->name.c_str(), gen.currentBlock());
    gen.locals()[id->name] = alloc;
    
    if (lhs != nullptr) {
        NAssignment assn(id, lhs);
        assn.accept(gen);
    }
    
}

/*
Value* NExternDeclaration::codeGen(CodeGenContext& context)
{
    vector<Type*> argTypes;
    VariableList::const_iterator it;
    for (it = arguments.begin(); it != arguments.end(); it++) {
        argTypes.push_back(typeOf((**it).type));
    }
    
    FunctionType *ftype = FunctionType::get(typeOf(type), makeArrayRef(argTypes), false);
    Function *function = Function::Create(ftype, GlobalValue::ExternalLinkage, id.name.c_str(), context.module);
    return function;
}
*/

void CodeGen::visit(NFunctionDeclaration& n)
{
    if(n.bindings->names.size() == n.bindings->type_sig.size()) {
        std::cerr << "Incomplete function declaration!" << std::endl;
        exit(1);
    }
    
    auto names = n.bindings->names;
    auto type_sig = n.bindings->type_sig;
    
    // Build vector of types
    vector<Type*> argTypes;
    IdentList::const_iterator it;
    for (uint i = 0; i < type_sig.size()-1; i++) {
        argTypes.push_back(typeOf(type_sig[i]));
    }
    
    // Build function
    FunctionType *ftype = FunctionType::get(typeOf(type_sig.back()), makeArrayRef(argTypes), false);
    Function* function = Function::Create(ftype, GlobalValue::InternalLinkage, names.front()->name.c_str(), module);
    BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", function, 0);
    
    pushBlock(bblock);
    
    Function::arg_iterator argsValues = function->arg_begin();
    Value* argumentValue;
    
    for (uint i = 1; i < names.size(); i++)
    {
        make_variable(*this, names[i], type_sig[i]);
        
        argumentValue = argsValues++;
        argumentValue->setName(names[i]->name.c_str());
        
        // Not sure what this is for
        StoreInst *inst = new StoreInst(argumentValue, locals()[names[i]->name], false, bblock);
    }
    
    n.block->accept(*this);
    ReturnInst::Create(getGlobalContext(), getCurrentReturnValue(), bblock);
    
    popBlock();
    std::cout << "Creating function: " << names.front()->name << endl;
    vvalue = function;
}