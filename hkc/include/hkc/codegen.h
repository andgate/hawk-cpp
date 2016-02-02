#ifndef _HKC_CODEGEN_H_
#define _HKC_CODEGEN_H_

#include <stack>
#include <typeinfo>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/raw_ostream.h>

#include "hkc/ast.h"

using namespace llvm;


class CodeGenBlock {
public:
    BasicBlock* block;
    Value* returnValue;
    std::map<std::string, Value*> locals;
};

class CodeGen : public ast::Visitor
{
private:
    Value* vvalue; // visit value to store visit output.
    std::stack<CodeGenBlock *> blocks;
    Function *mainFunction;
    
public:
    CodeGen()
    : module(new Module("main", getGlobalContext()))
    , builder(getGlobalContext())
    {}
    
    Module *module;
    IRBuilder<> builder;
    std::map<std::string, AllocaInst*> named_values;
    
    void build_module(ast::Module& root);
    void print_ir();
    void write_ir(const std::string& out_file);
    
    void visit(ast::Source& n) override;
    void visit(ast::Module& n) override;
    void visit(ast::Submodule& n) override;
    
    void visit(ast::Export& n) override;
    void visit(ast::QExport& n) override;
    void visit(ast::Import& n) override;
    void visit(ast::QImport& n) override;
    
    void visit(ast::IdentifierRef& n) override;
    void visit(ast::Integer& n) override;
    void visit(ast::Decimal& n) override;
    void visit(ast::String& n) override;
    void visit(ast::FunctionCall& n) override;
    void visit(ast::BinaryOperator& n) override;
    void visit(ast::Assignment& n) override;
    void visit(ast::Return& n) override;
    
    void visit(ast::Record& n) override;
    void visit(ast::TaggedUnion& n) override;
    void visit(ast::TaggedVariant& n) override;
    
    void visit(ast::Variable& n) override;
    void visit(ast::GlobalVariable& n) override;
    void visit(ast::LocalVariable& n) override;
    
    void visit(ast::Function& n) override;
    void visit(ast::GlobalFunction& n) override;
    void visit(ast::LocalFunction& n) override;
    
    void make_id_ref(ast::Identifier& n);
    
    
    
    BasicBlock *currentBlock() { return blocks.top()->block; }
    void pushBlock(BasicBlock *block) { blocks.push(new CodeGenBlock()); blocks.top()->returnValue = NULL; blocks.top()->block = block; }
    void popBlock() { CodeGenBlock *top = blocks.top(); blocks.pop(); delete top; }
    
    void setCurrentReturnValue(Value *value) { blocks.top()->returnValue = value; }
    Value* getCurrentReturnValue() { return blocks.top()->returnValue; }
    
};

#endif