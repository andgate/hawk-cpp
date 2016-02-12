#include "hkc/codegen.h"

#include <iostream>
#include <llvm/Support/raw_ostream.h>

using namespace std;

/* Compile the AST into a module */
void CodeGen::build_module(ast::Module& root)
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

/* Some static helper functions */

/* Returns an LLVM type based on the identifier */
static Type *typeOf(const ast::Identifier& type) 
{
    if (type.compare("i64") == 0) {
        return Type::getInt64Ty(getGlobalContext());
    }
    else if (type.compare("f64") == 0) {
        return Type::getDoubleTy(getGlobalContext());
    }
    
    return Type::getVoidTy(getGlobalContext());
}

static Constant* getDefault(const ast::Identifier& type)
{
    if (type.compare("i64") == 0) {
        return ConstantInt::get(getGlobalContext(), APInt(64, 0, true));
    }
    else if (type.compare("f64") == 0) {
        return ConstantFP::get(getGlobalContext(), APFloat(0.0));
    }
    
    std::cerr << "Unable to determine default of unknown type: " << type << std::endl;
    exit(EXIT_FAILURE);
}

static AllocaInst *CreateEntryBlockAlloca(Function *function, const std::string& var_id, const std::string& type_id)
{
    IRBuilder<> tmpB(&function->getEntryBlock(), function->getEntryBlock().begin());
    return tmpB.CreateAlloca(typeOf(type_id), 0, var_id.c_str());
}

/* -- Code Generation -- */

void CodeGen::visit(ast::RootModule& n) {}

void CodeGen::visit(ast::Module& n)
{
    std::cout << "Generating module " << ast::mk_id(n.id_path) << endl;
    Value *last = NULL;
    for(auto expr : n.exprs)
    {
        std::cout << "Generating code for " << typeid(*expr).name() << endl;
        expr->accept(*this);
        last = vvalue;
    }
    
    vvalue = last;
}

void CodeGen::visit(ast::Submodule& n) {}

void CodeGen::visit(ast::Import& n) {}

/*void CodeGen::visit(ast::Block& n)
{
    ast::pStatementVec::const_iterator it;
    Value* last = nullptr;
    for (it = n.statements.begin(); it != n.statements.end(); it++) {
        std::cout << "Generating code for " << typeid(**it).name() << endl;
        (**it).accept(*this);
        last = vvalue;
    }
    
    std::cout << "Creating block" << endl;
    vvalue = last;
}*/

void CodeGen::visit(ast::Integer& n)
{
    std::cout << "Creating integer: " << n.value << endl;
    vvalue = ConstantInt::get(Type::getInt64Ty(getGlobalContext()), n.value, true);
}

void CodeGen::visit(ast::Decimal& n)
{
    std::cout << "Creating double: " << n.value << endl;
    vvalue = ConstantFP::get(Type::getDoubleTy(getGlobalContext()), n.value);
}

void CodeGen::visit(ast::String& n)
{
    std::cout << "Creating string: " << n.value << endl;
    vvalue = ConstantFP::get(Type::getDoubleTy(getGlobalContext()), n.value);
}

void CodeGen::visit(ast::IdentifierRef& n)
{
    std::cout << "Creating identifier reference: " << n.str << endl;
    Value* v = named_values[n.str];
    if (!v) {
        std::cerr << "Variable not found " << n.str << endl;
        vvalue = nullptr;
        return;
    }
    
    vvalue = builder.CreateLoad(v, n.str.c_str());
}

/*static Value* loadIdent(CodeGen& gen, ast::pIdentifier& id)
{
    std::cout << "Creating identifier reference: " << id << endl;
    if (gen.locals().find(id) == gen.locals().end()) {
        std::cerr << "undeclared variable " << id << endl;
        return nullptr;
    }
    
    return new LoadInst(gen.locals()[id->name], "", false, gen.currentBlock());
}*/

void CodeGen::visit(ast::FunctionCall& n)
{
    Function *function = module->getFunction(n.id_ref->str);
    if (function != nullptr) {
        std::cerr << "no such function " << n.id_ref->str << endl;
        exit(0);
    }
    
    std::vector<Value*> args;
    ast::pExpressionVec::const_iterator it;
    for (it = n.args.begin(); it != n.args.end(); it++) {
        (**it).accept(*this);
        args.push_back(vvalue);
    }
    
    CallInst *call = CallInst::Create(function, makeArrayRef(args), "", currentBlock());
    std::cout << "Creating method call: " << n.id_ref->str << endl;
    
    vvalue = call;
}

void CodeGen::visit(ast::BinaryOperator& n)
{
    std::cout << "Creating binary operation " << n.op << endl;
    
    n.lhs->accept(*this);
    Value* lhs_val = vvalue;
    
    n.rhs->accept(*this);
    Value* rhs_val = vvalue;
    
    
    if(lhs_val == 0 || rhs_val == 0)
    {
        std::cerr << "Error generating expressions for bin op!" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    if(n.op == "+")
        vvalue = builder.CreateAdd(lhs_val, rhs_val);
    else if(n.op == "-")
        vvalue = builder.CreateSub(lhs_val, rhs_val);
    else if(n.op == "*")
        vvalue = builder.CreateMul(lhs_val, rhs_val);
    else if(n.op == "/")
        vvalue = builder.CreateSDiv(lhs_val, rhs_val);
    else
    {
        std::cerr << "Unknown binary operator." << std::endl;
        exit(EXIT_FAILURE);
    }
}

void CodeGen::visit(ast::Assignment& n)
{
    /*std::cout << "Creating assignment for " << n.lhs->str << endl;
    if (locals().find(n.lhs->str) == locals().end()) {
        std::cerr << "undeclared variable " << n.lhs->str << endl;
        vvalue = nullptr;
    }
    
    n.rhs->accept(*this);
    Value* rhs_val = vvalue;
    
    vvalue = new StoreInst(rhs_val, locals()[n.lhs->str], false, currentBlock());*/
}

void CodeGen::visit(ast::Return& n)
{
    std::cout << "Generating return code for " << typeid(*n.expression).name() << endl;
    n.expression->accept(*this);
    setCurrentReturnValue(vvalue);
}

void CodeGen::visit(ast::Variable& n) {}

void CodeGen::visit(ast::GlobalVariable& n)
{
    GlobalVariable* gvar_ptr = new GlobalVariable(*module,typeOf(n.type),false,
                                                  GlobalValue::CommonLinkage,
                                                  getDefault(n.type),Twine(n.id));
}

void CodeGen::visit(ast::Record& n) {}
void CodeGen::visit(ast::TaggedUnion& n) {}
void CodeGen::visit(ast::TaggedVariant& n) {}

void CodeGen::visit(ast::LocalVariable& n)
{
    std::cout << "Creating variable declaration " << n.id << ": " << n.type << endl;
    
    Value* initV;
    
    if(n.rhs) {
        n.rhs->accept(*this);
        initV = vvalue;
    } else {
        // Choose a default based on the type
    }
    
    Function* function = builder.GetInsertBlock()->getParent();
    AllocaInst* alloca = CreateEntryBlockAlloca(function, n.id, n.type);
    builder.CreateStore(initV, alloca, false);
    
    //vvalue = alloc;
}

/*
Value CodeGen::visit(ast::ExternDeclaration& p)
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

void CodeGen::visit(ast::Function& n) {}
void CodeGen::visit(ast::LocalFunction& n) {}

void CodeGen::visit(ast::GlobalFunction& n)
{
    /*std::cout << "Creating function: " << n.id << std::endl;
    
    // Build vector of types
    vector<Type*> argTypes;
    for (auto param : n.params) {
        argTypes.push_back(typeOf(param->type));
    }
    
    // Build function
    FunctionType *ftype = FunctionType::get(typeOf(n.type), makeArrayRef(argTypes), false);
    Function* function = Function::Create(ftype, GlobalValue::InternalLinkage, n.id.c_str(), module);
    
    // Start function block
    BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", function, 0);
    pushBlock(bblock);
    
    Function::arg_iterator argsValues = function->arg_begin();
    Value* argumentValue;
    
    for (auto param : n.params)
    {
        param->accept(*this);
        
        // What is this for??
        argumentValue = argsValues++;
        argumentValue->setName(param->id.c_str());
        StoreInst *inst = new StoreInst(argumentValue, [param->id], false, bblock);
    }
    
    for(auto expr : n.exprs)
        expr->accept(*this);
    
    ReturnInst::Create(getGlobalContext(), getCurrentReturnValue(), bblock);
    
    popBlock();
    vvalue = function;*/
}