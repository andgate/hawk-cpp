#include "hkc/parser/node.h"
#include "hkc/codegen.h"
#include "parser.hpp"

using namespace std;

/* Compile the AST into a module */
void CodeGen::generateCode(NBlock& root)
{
    std::cout << "Generating code...\n";
    
    /* Create the top level interpreter function to call as entry */
    vector<Type*> argTypes;
    FunctionType *ftype = FunctionType::get(Type::getVoidTy(getGlobalContext()), makeArrayRef(argTypes), false);
    mainFunction = Function::Create(ftype, GlobalValue::InternalLinkage, "main", module);
    BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", mainFunction, 0);
    
    /* Push a new variable/block context */
    pushBlock(bblock);
    root.accept(*this); /* emit bytecode for the toplevel block */
    ReturnInst::Create(getGlobalContext(), bblock);
    popBlock();
    
    /* Print the bytecode in a human-readable format 
     *      to see if our program compiled properly
     */
    std::cout << "Code is generated.\n";
    PassManager<Module> pm;
    pm.addPass(PrintModulePass(outs()));
    pm.run(*module);
}

/* Executes the AST by running the main function */
GenericValue CodeGen::runCode() {
    std::cout << "Running code...\n";
    ExecutionEngine *ee = EngineBuilder( unique_ptr<Module>(module) ).create();
    ee->finalizeObject();
    vector<GenericValue> noargs;
    GenericValue v = ee->runFunction(mainFunction, noargs);
    std::cout << "Code was run.\n";
    return v;
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
    std::cout << "Creating variable declaration " << n.type->name << " " << n.id->name << endl;
    AllocaInst *alloc = new AllocaInst(typeOf(n.type), n.id->name.c_str(), currentBlock());
    locals()[n.id->name] = alloc;
    if (n.assignmentExpr != nullptr) {
        NAssignment assn(n.id, n.assignmentExpr);
        assn.accept(*this);
    }
    
    vvalue = alloc;
}

static void make_variable(CodeGen& gen, nident id, nident type, nexpr assignmentExpr = nullptr)
{
    std::cout << "Creating variable declaration " << type->name << " " << id->name << endl;
    AllocaInst *alloc = new AllocaInst(typeOf(type), id->name.c_str(), gen.currentBlock());
    gen.locals()[id->name] = alloc;
    
    if (assignmentExpr != nullptr) {
        NAssignment assn(id, assignmentExpr);
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
    // Build vector of types
    vector<Type*> argTypes;
    IdentList::const_iterator it;
    for (int i = 0; i < n.type_sig.size()-1; i++) {
        argTypes.push_back(typeOf(n.type_sig[i]));
    }
    
    // Build function
    FunctionType *ftype = FunctionType::get(typeOf(n.type_sig.back()), makeArrayRef(argTypes), false);
    Function* function = Function::Create(ftype, GlobalValue::InternalLinkage, n.id->name.c_str(), module);
    BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", function, 0);
    
    pushBlock(bblock);
    
    Function::arg_iterator argsValues = function->arg_begin();
    Value* argumentValue;
    
    assert(n.params.size() == n.type_sig.size() - 1);
    for (int i = 0; i < n.params.size(); i++)
    {
        make_variable(*this, n.params[i], n.type_sig[i]);
        
        argumentValue = argsValues++;
        argumentValue->setName(n.params[i]->name.c_str());
        StoreInst *inst = new StoreInst(argumentValue, locals()[n.params[i]->name], false, bblock);
    }
    
    n.block->accept(*this);
    ReturnInst::Create(getGlobalContext(), getCurrentReturnValue(), bblock);
    
    popBlock();
    std::cout << "Creating function: " << n.id->name << endl;
    vvalue = function;
}