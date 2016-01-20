#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <llvm/IR/Value.h>

#include <memory>

class NStatement;
class NExpression;
class NIdentifier;
class NBlock;
class NInteger;
class NDecimal;
class NString;
class NFunctionCall;
class NBinaryOperator;
class NAssignment;
class NStatementExpression;
class NExpressionStatement;
class NReturnStatement;
class NVariableDeclaration;
class NFunctionDeclaration;

typedef std::shared_ptr<NBlock> nblock;
typedef std::shared_ptr<NStatement> nstmt;
typedef std::shared_ptr<NIdentifier> nident;
typedef std::shared_ptr<NExpression> nexpr;

typedef std::vector<std::shared_ptr<NStatement>> StatementList;
typedef std::vector<std::shared_ptr<NExpression>> ExpressionList;
typedef std::vector<std::shared_ptr<NIdentifier>> IdentList;

class Visitor;

class Node {
public:
    virtual ~Node() {}
    
    virtual void accept(Visitor& v) = 0;
};

class Visitor
{
public:
    virtual void visit(NExpression& n) = 0;
    virtual void visit(NStatement& n) = 0;
    virtual void visit(NInteger& n) = 0;
    virtual void visit(NDecimal& n) = 0;
    virtual void visit(NString& n) = 0;
    virtual void visit(NIdentifier& n) = 0;
    virtual void visit(NFunctionCall& n) = 0;
    virtual void visit(NBinaryOperator& n) = 0;
    virtual void visit(NAssignment& n) = 0;
    virtual void visit(NBlock& n) = 0;
    virtual void visit(NExpressionStatement& n) = 0;
    virtual void visit(NStatementExpression& n) = 0;
    virtual void visit(NReturnStatement& n) = 0;
    virtual void visit(NVariableDeclaration& n) = 0;
    virtual void visit(NFunctionDeclaration& n) = 0;
};

class NExpression : public Node {
public:
    virtual void accept(Visitor &v) { v.visit(*this); }
};

class NStatement : public Node {
public:
    IdentList options;
    
    NStatement()
    : options()
    {}
    
    virtual void accept(Visitor &v) { v.visit(*this); }
};

class NInteger : public NExpression {
public:
    std::string value;
    NInteger(const std::string& value) : value(value) { }
    
    virtual void accept(Visitor &v) { v.visit(*this); }
};

class NDecimal : public NExpression {
public:
    std::string value;
    NDecimal(const std::string& value) : value(value) { }
    
    virtual void accept(Visitor &v) { v.visit(*this); }
};

class NString : public NExpression {
public:
    std::string value;
    NString(const std::string& value) : value(value) {}
    
    virtual void accept(Visitor &v) {v.visit(*this); }
};

class NIdentifier : public NExpression {
public:
    std::string name;
    
    NIdentifier(const std::string& name) : name(name) { }
    
    virtual void accept(Visitor &v) { v.visit(*this); }
};

class NFunctionCall : public NExpression {
public:
    nident id;
    ExpressionList arguments;
    
    NFunctionCall(nident id) : id(id), arguments() { }
    NFunctionCall(nident id, ExpressionList arguments)
    : id(id), arguments(arguments) { }
    
    virtual void accept(Visitor &v) { v.visit(*this); }
};

class NBinaryOperator : public NExpression {
public:
    nexpr lhs;
    std::string op;
    nexpr rhs;
    
    NBinaryOperator(nexpr lhs, std::string op, nexpr rhs)
    : lhs(lhs), op(op), rhs(rhs)
    { }
    
    virtual void accept(Visitor &v) { v.visit(*this); }
};

class NAssignment : public NExpression {
public:
    nident lhs;
    nexpr rhs;
    NAssignment(nident lhs, nexpr rhs) : 
    lhs(lhs), rhs(rhs) { }

    virtual void accept(Visitor &v) { v.visit(*this); }
};

class NBlock : public NExpression {
public:
    StatementList statements;
    NBlock() : statements() { }
    
    virtual void accept(Visitor &v) { v.visit(*this); }
};

class NStatementExpression : public NExpression {
public:
    nstmt statement;
    NStatementExpression(nstmt statement)
    : statement(statement) { }
    
    virtual void accept(Visitor &v) { v.visit(*this); }
};

class NExpressionStatement : public NStatement {
public:
    nexpr expression;
    NExpressionStatement(nexpr expression)
    : NStatement(), expression(expression) { }
    
    virtual void accept(Visitor &v) { v.visit(*this); }
};

class NReturnStatement : public NStatement {
public:
    nexpr expression;
    NReturnStatement(nexpr expression)
    : NStatement(), expression(expression) { }
    
    virtual void accept(Visitor &v) { v.visit(*this); }
};

class NVariableDeclaration : public NStatement {
public:
    nident id;
    nident type;
    nexpr lhs;
    
    NVariableDeclaration(nident id)
    : NStatement(), id(id), type(nullptr), lhs(nullptr) { }
    
    NVariableDeclaration(nident id, nident type)
    : NStatement(), id(id), type(type), lhs(nullptr) { }
    
    NVariableDeclaration(nident id, nident type, nexpr lhs)
    : NStatement(), id(id), type(type), lhs(lhs) { }
    
    virtual void accept(Visitor &v) { v.visit(*this); }
};

class NFunctionDeclaration : public NStatement {
public:
    nident id;
    IdentList params;
    IdentList type_sig;
    nblock block;
    
    NFunctionDeclaration(nident id, IdentList params, IdentList type_sig, nblock block)
    : NStatement(), id(id), params(params), type_sig(type_sig), block(block) {}
    
    virtual void accept(Visitor &v) { v.visit(*this); }
};