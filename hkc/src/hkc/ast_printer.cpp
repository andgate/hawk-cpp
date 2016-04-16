#include "hkc/ast_printer.h"
#include <iostream>
#include <string>

using namespace std;
using namespace ast;


void ast::print(pModule& root)
{
    Printer printer;
    root->accept(printer);
}

void Printer::visit(Module& n)
{        
    cout << "Module" << endl;
    indent();
        cout << ws << "- id_path: " << mk_id(n.id_path) << endl;
        cout << ws << "- exprs:" << endl;
        indent();
            for(auto expr : n.exprs) expr->accept(*this);
        undent();
    undent();
}

void Printer::visit(Import& n)
{
    cout << ws << "Import" << endl;
    indent();
        cout << ws << "- id_paths: " << endl;
        indent();
            for(auto id_path : n.id_paths) cout << ws << mk_id(id_path) << endl;
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
        cout << ws << "- args:" << endl;
        indent();
            for(auto arg : n.args) arg->accept(*this);
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
        cout << ws << "- exprs:" << endl;
        indent();
            for(auto expr : n.exprs) expr->accept(*this);
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
        
        cout << ws << "- stmts:" << endl;
        indent();
            for(auto stmt : n.stmts) stmt->accept(*this);
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
        
        cout << ws << "- stmts:" << endl;
        indent();
            for(auto stmt : n.stmts) stmt->accept(*this);
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
    
    cout << ws << "- stmts:" << endl;
    indent();
        for(auto stmt : n.stmts) stmt->accept(*this);
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