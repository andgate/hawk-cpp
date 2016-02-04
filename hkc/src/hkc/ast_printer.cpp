#ifndef _CST_PRINTER_H_
#define _CST_PRINTER_H_

#include "hkc/ast_printer.h"

#include <iostream>
#include <string>

using namespace std;
using namespace ast;


void Printer::visit(ExpressionGroup& n)
{
    cout << ws << "ExpressionGroup" << endl;
    indent();
        cout << ws << "- exprs:" << endl;
        indent();
            for(auto expr : n.exprs) expr->accept(*this);
        undent();
    undent();
}

void Printer::visit(Source& n)
{        
    for(auto module : n.modules)
    {
        module->accept(*this);
    }
}

void Printer::visit(Module& n)
{        
    cout << "Module" << endl;
    indent();
        cout << ws << "- id: " << n.id << endl;
        cout << ws << "- groups:" << endl;
        indent();
            n.group->accept(*this);
        undent();
    undent();
}

void Printer::visit(Submodule& n)
{
    cout << ws << "Submodule" << endl;
    indent();
        cout << ws << "- id: " << n.id << endl;
        cout << ws << "- groups:" << endl;
        indent();
            n.group->accept(*this);
        undent();
    undent();
}

void Printer::visit(Import& n)
{
    cout << ws << "Import" << endl;
    indent();
        cout << ws << "- id: " << n.id->id << endl;
        cout << ws << "- subs:" << endl;
        indent();
            for(auto sub : n.id->subs)
                cout << ws << sub << endl;
        undent();
    undent();
}

void Printer::visit(QImport& n)
{
    cout << ws << "QImport" << endl;
    indent();
        cout << ws << "- id: " << n.id->id << endl;
        cout << ws << "- subs:" << endl;
        indent();
            for(auto sub : n.id->subs)
                cout << ws << sub << endl;
        undent();
    undent();
}

void Printer::visit(IdentifierRef& n)
{
    cout << ws << "IdentifierRef" << endl;
    indent();
        cout << ws << "- str: " << n.str << endl;
    undent();
}

void Printer::visit(Integer& n)
{
    cout << ws << "Integer" << endl;
    indent();
        cout << ws << "- value: " << n.value << endl;
    undent();
}

void Printer::visit(Decimal& n)
{
    cout << ws << "Decimal" << endl;
    indent();
        cout << ws << "- value: " << n.value << endl;
    undent();
}

void Printer::visit(String& n)
{
    cout << ws << "String" << endl;
    indent();
        cout << ws << "- value: " << n.value << endl;
    undent();
}

void Printer::visit(FunctionCall& n)
{
    cout << ws << "FunctionCall" << endl;
    indent();
        cout << ws << "- id_ref:" << endl;
        indent();
            n.id_ref->accept(*this);
        undent();
        cout << ws << "- arguments:" << endl;
        indent();
            n.arguments->accept(*this);
        undent();
    undent();
}

void Printer::visit(BinaryOperator& n)
{   
    cout << ws << "BinaryOperator" << endl;
    indent();
        cout << ws << "- op: " << n.op << endl;
        cout << ws << "- lhs:" << endl;
        indent();
            n.lhs->accept(*this);
        undent();
        cout << ws << "- rhs:" << endl;
        indent();
            n.rhs->accept(*this);
        undent();
    undent();
}

void Printer::visit(Assignment& n)
{
    cout << ws << "Assignment" << endl;
    indent();
        cout << ws << "- lhs:" << endl;
        indent();
            n.lhs->accept(*this);
        undent();
        cout << ws << "- rhs:" << endl;
        indent();
            n.rhs->accept(*this);
        undent();
    undent();
}

void Printer::visit(Return& n)
{
    cout << ws << "Return" << endl;
    indent();
        cout << ws << "- expression:" << endl;
        indent();
            n.expression->accept(*this);
        undent();
    undent();
}

void Printer::visit(Record& n)
{
    cout << ws << "Record" << endl;
    indent();
        cout << ws << "- id: " << n.id << endl;
        cout << ws << "- group:" << endl;
        indent();
            for(auto expr : n.group->exprs) expr->accept(*this);
        undent();
    undent();
}

void Printer::visit(TaggedUnion& n)
{
    cout << ws << "TaggedVariant" << endl;
    indent();
        cout << ws << "- id: " << n.id << endl;
        indent();
            for(auto variant : n.variants) variant->accept(*this);
        undent();
    undent();
}

void Printer::visit(TaggedVariant& n)
{
    cout << ws << "TaggedVariant" << endl;
    indent();
        cout << ws << "- tag: " << n.tag << endl;
        cout << ws << "- types:" << endl;
        indent();
            for(auto type : n.types)
                cout << ws << type << endl;
        undent();
    undent();
}

void Printer::visit(Variable& n)
{
    cout << ws << "LocalVariable" << endl;
    
    indent();
    
        cout << ws << "- id: " << n.id << endl;
        
        if(!n.type.empty())
            cout << ws << "- type: " << n.type << endl;
        
        if(n.rhs != nullptr)
        {
            cout << ws << "- rhs: " << endl;
            indent();
                n.rhs->accept(*this);
            undent();
        }
    
    undent();
}

void Printer::visit(GlobalVariable& n)
{
    cout << ws << "GlobalVariable" << endl;
    
    indent();
        cout << ws << "- id: " << n.id << endl;
        
        if(!n.type.empty())
            cout << ws << "- type: " << n.type << endl;
        
        if(n.rhs != nullptr)
        {
            cout << ws << "- rhs: " << endl;
            indent();
                n.rhs->accept(*this);
            undent();
        }
    
    undent();
}

void Printer::visit(LocalVariable& n)
{
    cout << ws << "LocalVariable" << endl;
    
    indent();
    
        cout << ws << "- id: " << n.id << endl;
        
        if(!n.type.empty())
            cout << ws << "- type: " << n.type << endl;
        
        if(n.rhs != nullptr)
        {
            cout << ws << "- rhs: " << endl;
            indent();
                n.rhs->accept(*this);
            undent();
        }
    
    undent();
}

void Printer::visit(Function& n)
{
    cout << ws << "Function" << endl;
    
    indent();
    
        cout << ws << "- id: " << n.id << endl;
        
        if(!n.type.empty())
            cout << ws << "- type: " << n.type << endl;
        
        if(!n.params.empty())
        {
            cout << ws << "- params:" << endl;
            indent();
                for(auto param : n.params)
                    param->accept(*this);
            undent();
        }
        
        cout << ws << "- statements:" << endl;
        indent();
        n.statements->accept(*this);
        undent();
    
    undent();
}

void Printer::visit(GlobalFunction& n)
{
    cout << ws << "GlobalFunction" << endl;
    
    indent();
    
        cout << ws << "- id: " << n.id << endl;
        
        if(!n.type.empty())
            cout << ws << "- type: " << n.type << endl;
        
        if(!n.params.empty())
        {
            cout << ws << "- params:" << endl;
            indent();
                for(auto param : n.params)
                    param->accept(*this);
            undent();
        }
        
        cout << ws << "- statements:" << endl;
        indent();
            n.statements->accept(*this);
        undent();
    
    undent();
}

void Printer::visit(LocalFunction& n)
{
    cout << ws << "LocalFunction" << endl;
    
    indent();
    
    cout << ws << "- id: " << n.id << endl;
    
    if(!n.type.empty())
        cout << ws << "- type: " << n.type << endl;
    
    if(!n.params.empty())
    {
        cout << ws << "- params:" << endl;
        indent();
        for(auto param : n.params)
            param->accept(*this);
        undent();
    }
    
    cout << ws << "- statements:" << endl;
    indent();
    n.statements->accept(*this);
    undent();
    
    undent();
}

void Printer::indent()
{
    curr_indent += 2;
    ws = std::string(curr_indent, ' ');
}

void Printer::undent()
{
    curr_indent -= 2;
    ws = std::string(curr_indent, ' ');
}

#endif // _CST_PRINTER_H_