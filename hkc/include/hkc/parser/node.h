#ifndef HAWK_NODE_H
#define HAWK_NODE_H

#include <iostream>
#include <vector>
#include <string>
#include <llvm/IR/Value.h>

#include <memory>

class NStatement;
class NExpression;
class NIdentifier;
class NBlock;
class NModule;
class NInteger;
class NDecimal;
class NString;
class NFunctionCall;
class NBinaryOperator;
class NAssignment;
class NStatementExpression;
class NExpressionStatement;
class NReturnStatement;
class NameBindings;
class NVariableDeclaration;
class NFunctionDeclaration;

typedef std::shared_ptr<NModule> nmodule;
typedef std::shared_ptr<NBlock> nblock;
typedef std::shared_ptr<NStatement> nstmt;
typedef std::shared_ptr<NIdentifier> nident;
typedef std::shared_ptr<NExpression> nexpr;
typedef std::shared_ptr<NameBindings> nbindings;

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
    virtual void visit(NBlock& n) = 0;
    virtual void visit(NModule& n) = 0;
    virtual void visit(NInteger& n) = 0;
    virtual void visit(NDecimal& n) = 0;
    virtual void visit(NString& n) = 0;
    virtual void visit(NIdentifier& n) = 0;
    virtual void visit(NFunctionCall& n) = 0;
    virtual void visit(NBinaryOperator& n) = 0;
    virtual void visit(NAssignment& n) = 0;
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

class NModule : public NExpression
{
public:
    std::string name;
    nblock block;
    
    NModule(const std::string& name, nblock block)
    : name(name), block(block) { }
    
    virtual void accept(Visitor &v) { v.visit(*this); }
};

class NBlock : public NExpression {
public:
    StatementList statements;
    NBlock() : statements() { }
    
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

class NameBindings {
public:
    IdentList names;
    IdentList type_sig;
    
    NameBindings(IdentList names, IdentList type_sig)
    : names(names), type_sig(type_sig) {}
};

class NVariableDeclaration : public NStatement {
public:
    nbindings bindings;
    nexpr lhs;
    
    NVariableDeclaration(nbindings bindings)
    : NStatement(), bindings(bindings), lhs(nullptr) { }
    
    NVariableDeclaration(nbindings bindings, nexpr lhs)
    : NStatement(), bindings(bindings), lhs(lhs) { }
    
    virtual void accept(Visitor &v) { v.visit(*this); }
};

class NFunctionDeclaration : public NStatement {
public:
    nbindings bindings;
    nblock block;
    
    NFunctionDeclaration(nbindings bindings, nblock block)
    : NStatement(), bindings(bindings), block(block) {}
    
    virtual void accept(Visitor &v) { v.visit(*this); }
};


#endif