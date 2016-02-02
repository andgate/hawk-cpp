#ifndef _CST_PRINTER_H_
#define _CST_PRINTER_H_

#include "hkc/ast_printer.h"

#include <iostream>

using namespace std;
using namespace ast;

void Printer::visit(Source& n)
{        
    for(auto module : n.modules)
    {
        module->accept(*this);
    }
}

void Printer::visit(Module& n)
{        
    cout << "Module " << n.id << endl;
    for(auto expr : n.exprs)
        expr->accept(*this);
}

void Printer::visit(Submodule& n) {}

void Printer::visit(Export& n) {}
void Printer::visit(QExport& n) {}
void Printer::visit(Import& n) {}
void Printer::visit(QImport& n) {}

void Printer::visit(IdentifierRef& n)
{
    cout << n.str;
}

void Printer::visit(Integer& n)
{
    cout << n.value;
}

void Printer::visit(Decimal& n)
{
    cout << n.value;
}

void Printer::visit(String& n)
{
    cout << n.value;
}

void Printer::visit(FunctionCall& n)
{
    cout << n.id_ref->str;
    for(auto &arg : n.arguments)
    {
        cout << " ";
        arg->accept(*this);
    }
}

void Printer::visit(BinaryOperator& n)
{   
    n.lhs->accept(*this);
    cout << " " << n.op << " ";
    n.rhs->accept(*this);
}

void Printer::visit(Assignment& n)
{
    cout << n.lhs->str << " = ";
    n.rhs->accept(*this);
}

void Printer::visit(Return& n)
{
    cout << "^ ";
    
    n.expression->accept(*this);
    
    cout << ";" << endl;
}

void Printer::visit(Record& n)
{
    for(auto expr : n.exprs)
    {
        expr->accept(*this);
    }
}

void Printer::visit(TaggedUnion& n)
{
    for(auto variant : n.variants)
    {
        variant->accept(*this);
    }
}

void Printer::visit(TaggedVariant& n) {}

void Printer::visit(Variable& n) {}

void Printer::visit(GlobalVariable& n)
{
    cout << n.id;
    if(!n.type.empty())
    {
        cout << ": " << n.type;
    }
    
    cout << " <- ";
    n.rhs->accept(*this);
    cout << ";" << endl;
}

void Printer::visit(LocalVariable& n)
{
    cout << n.id;
    if(!n.type.empty())
    {
        cout << ": " << n.type;
    }
    
    cout << " <- ";
    n.rhs->accept(*this);
    cout << ";" << endl;
}

void Printer::visit(Function& n) {}

void Printer::visit(GlobalFunction& n)
{
    // Disabling attributes for now...
    /*if(!n.attributes.empty())
     *   {
     *       cout << "@";
     *       for(auto attribute : n.attributes)
     *       {
     *           cout << " " << attribute->str;
}

cout << endl;
}*/
    
    cout << n.id << " ";
    for(auto param : n.params)
        cout << param->id << " ";
    
    cout << ": ";
    for(auto param : n.params)
        cout << param->type << " -> ";
    
    cout << n.type;
    
    cout << " := { " << endl;
    
    for(auto expr : n.exprs)
        expr->accept(*this);
    
    cout << "}" << endl;
}

void Printer::visit(LocalFunction& n)
{
    // Disabling attributes for now...
    /*if(!n.attributes.empty())
    {
        cout << "@";
        for(auto attribute : n.attributes)
        {
            cout << " " << attribute->str;
        }
        
        cout << endl;
    }*/
    
    cout << n.id << " ";
    for(auto param : n.params)
        cout << param->id << " ";
    
    cout << ": ";
    for(auto param : n.params)
        cout << param->type << " -> ";
    
    cout << n.type;
    
    cout << " := { " << endl;
    
    for(auto expr : n.exprs)
        expr->accept(*this);
    
    cout << "}" << endl;
}

#endif // _CST_PRINTER_H_