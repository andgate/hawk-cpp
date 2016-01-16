#pragma once

#include "hkc/parser/node.h"

#include <iostream>

using namespace std;

class NodePrinter : public Visitor
{
public:    
    void visit(NExpression& n)
    {
        cout << "ERROR: NodePrinter should never visit NExpression!" << endl;
    }
    
    void visit(NStatement& n)
    {
        cout << "ERROR: NodePrinter visiting NStatement?" << endl;
    }
    
    void visit(NInteger& n)
    {
        cout << n.value;
    }
    
    void visit(NDecimal& n)
    {
        cout << n.value;
    }
    
    void visit(NIdentifier& n)
    {
        cout << n.name;
    }
    
    void visit(NFunctionCall& n)
    {
        cout << n.id;
        for(auto &exp : n.arguments)
        {
            cout << " ";
            exp->accept(*this);
        }
    }
    
    void visit(NBinaryOperator& n)
    {   
        n.lhs->accept(*this);
        cout << " " << n.op << " ";
        n.rhs->accept(*this);
    }
    
    void visit(NAssignment& n)
    {
        n.lhs->accept(*this);
        cout << " = ";
        n.rhs->accept(*this);
    }
    
    void visit(NBlock& n)
    {        
        for(auto statement : n.statements)
        {
            statement->accept(*this);
        }
    }
    
    void visit(NExpressionStatement& n)
    {
        n.expression->accept(*this);
        cout << ";" << endl;
    }
    
    void visit(NStatementExpression& n)
    {
        n.statement->accept(*this);
    }
    
    void visit(NReturnStatement& n)
    {
        cout << "return ";
        
        n.expression->accept(*this);
        
        cout << ";" << endl;
    }
    
    void visit(NVariableDeclaration& n)
    {
        cout << "Variable declarations are unused until futher notice!";
    }
    
    void visit(NFunctionDeclaration& n)
    {
        if(!n.options.empty())
        {
            cout << "@";
            for(auto &option : n.options)
            {
                cout << " " << option->name;
            }
            
            cout << endl;
        }
        
        for(auto &id : n.header)
        {
            cout << id->name << " "; 
        }
        
        if(!n.type.empty())
        {   
            cout << ": " << n.type.front()->name << " ";
            
            if(n.type.size() > 1)
            {
                for(auto it = n.type.begin()+1; it != n.type.end(); it++)
                {
                    cout << "-> " << (**it).name << " ";
                }
            }
        }
        
        cout << ":= { " << endl;
        
        n.block->accept(*this);
        
        cout << "}" << endl;
    }
    
};