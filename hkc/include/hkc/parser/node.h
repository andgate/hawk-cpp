#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <llvm/IR/Value.h>

#include <memory>

class CodeGenContext;
class NStatement;
class NExpression;
class NIdentifier;
class NBlock;

typedef std::shared_ptr<NBlock> nblock;
typedef std::shared_ptr<NStatement> nstmt;
typedef std::shared_ptr<NIdentifier> nident;
typedef std::shared_ptr<NExpression> nexpr;

typedef std::vector<std::shared_ptr<NStatement>> StatementList;
typedef std::vector<std::shared_ptr<NExpression>> ExpressionList;
typedef std::vector<std::shared_ptr<NIdentifier>> IdentList;

class Node {
public:
    virtual ~Node() {}
    virtual llvm::Value* codeGen(CodeGenContext& context) { }
};

class NExpression : public Node {
};

class NStatement : public Node {
};

class NInteger : public NExpression {
public:
    long long value;
    NInteger(long long value) : value(value) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NDouble : public NExpression {
public:
    double value;
    NDouble(double value) : value(value) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NIdentifier : public NExpression {
public:
    std::string name;
    NIdentifier(const std::string& name) : name(name) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NFunctionCall : public NExpression {
public:
    nident id;
    ExpressionList arguments;
    NFunctionCall(nident id, ExpressionList arguments) :
    id(id), arguments(arguments) { }
    NFunctionCall(nident id) : id(id) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NBinaryOperator : public NExpression {
public:
    std::string op;
    nexpr lhs;
    nexpr rhs;
    
    NBinaryOperator(nexpr lhs, std::string op, nexpr rhs)
    : op(op), lhs(lhs), rhs(rhs)
    { }
    
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NAssignment : public NExpression {
public:
    nident lhs;
    nexpr rhs;
    NAssignment(nident lhs, nexpr rhs) : 
    lhs(lhs), rhs(rhs) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NBlock : public NExpression {
public:
    StatementList statements;
    NBlock() { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NExpressionStatement : public NStatement {
public:
    nexpr expression;
    NExpressionStatement(nexpr expression) : 
    expression(expression) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NReturnStatement : public NStatement {
public:
    nexpr expression;
    NReturnStatement(nexpr expression) : 
    expression(expression) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NVariableDeclaration : public NStatement {
public:
    nident type;
    nident id;
    nexpr assignmentExpr;
    NVariableDeclaration(nident type, nident id) :
    type(type), id(id) { }
    NVariableDeclaration(nident type, nident id, nexpr assignmentExpr) :
    type(type), id(id), assignmentExpr(assignmentExpr) { }
    virtual llvm::Value* codeGen(CodeGenContext& context);
};

class NFunctionDeclaration : public NStatement {
public:
    IdentList header;
    IdentList type;
    nblock block;
    
    NFunctionDeclaration(IdentList header,
                         nblock block,
                         IdentList type)
    : header(header), type(type), block(block)
    { }
    
    NFunctionDeclaration(const IdentList& header, nblock block)
    : header(header), type(), block(block)
    { }
    
    virtual llvm::Value* codeGen(CodeGenContext& context);
};