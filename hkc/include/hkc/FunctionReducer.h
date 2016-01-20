#pragma once

#include "hkc/parser/node.h"

class Visitor
{
public:
    virtual void visit(NExpression& n)
    {
    }
    
    virtual void visit(NStatement& n)
    {
    }
    
    virtual void visit(NInteger& n)
    {
    }
    
    virtual void visit(NDecimal& n)
    {
    }
    
    virtual void visit(NString& n)
    {
    }
    
    virtual void visit(NIdentifier& n)
    {
    }
    
    virtual void visit(NFunctionCall& n)
    {
    }
    
    virtual void visit(NBinaryOperator& n)
    {
    }
    
    virtual void visit(NAssignment& n)
    {
    }
    
    virtual void visit(NBlock& n)
    {
    }
    
    virtual void visit(NExpressionStatement& n)
    {
    }
    
    virtual void visit(NStatementExpression& n)
    {
    }
    
    virtual void visit(NReturnStatement& n)
    {
    }
    
    virtual void visit(NVariableDeclaration& n)
    {
    }
    
    virtual void visit(NFunctionDeclaration& n)
    {
        if(n.type_sig.size() == 0 && n.params.size() == 0)
        {
            // Reduce to variable
        }
    }
};