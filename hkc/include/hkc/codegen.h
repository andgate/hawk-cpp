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

#include "hkc/parser/node.h"

using namespace llvm;

class NBlock;

class CodeGenBlock {
public:
    BasicBlock* block;
    Value* returnValue;
    std::map<std::string, Value*> locals;
};

class CodeGen : public Visitor
{
private:
    Value* vvalue; // visit value to store visit output.
    std::stack<CodeGenBlock *> blocks;
    Function *mainFunction;
    
public:
    CodeGen() { module = new Module("main", getGlobalContext()); }
    
    Module *module;
    
    std::map<std::string, Value*>& locals() { return blocks.top()->locals; }
    
    BasicBlock *currentBlock() { return blocks.top()->block; }
    void pushBlock(BasicBlock *block) { blocks.push(new CodeGenBlock()); blocks.top()->returnValue = NULL; blocks.top()->block = block; }
    void popBlock() { CodeGenBlock *top = blocks.top(); blocks.pop(); delete top; }
    
    void setCurrentReturnValue(Value *value) { blocks.top()->returnValue = value; }
    Value* getCurrentReturnValue() { return blocks.top()->returnValue; }
    
    void generateCode(NBlock& root);
    GenericValue runCode();
    
    
    void visit(NExpression& n) override;
    void visit(NStatement& n) override;
    void visit(NInteger& n) override;
    void visit(NDecimal& n) override;
    void visit(NString& n) override;
    void visit(NIdentifier& n) override;
    void visit(NFunctionCall& n) override;
    void visit(NBinaryOperator& n) override;
    void visit(NAssignment& n) override;
    void visit(NBlock& n) override;
    void visit(NExpressionStatement& n) override;
    void visit(NStatementExpression& n) override;
    void visit(NReturnStatement& n) override;
    void visit(NVariableDeclaration& n) override;
    void visit(NFunctionDeclaration& n) override;
    
};