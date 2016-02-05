#include "hkc/SymbolTableGen.h"

using namespace ast;

void SymbolTableGen::visit(ExpressionGroup& n)
{
    for(auto expr : n.exprs)
    {
        expr->accept(*this);
    }
}

void SymbolTableGen::visit(Source& n)
{
    for(auto module : n.modules)
    {
        module->accept(*this);
    }
}

void SymbolTableGen::visit(Module& n)
{
    n.group->accept(*this);
}

void SymbolTableGen::visit(Submodule& n)
{
    n.group->accept(*this);
}

void SymbolTableGen::visit(Import& n) {}
void SymbolTableGen::visit(QImport& n) {}

void SymbolTableGen::visit(IdentifierRef& n) {}
void SymbolTableGen::visit(Integer& n) {}
void SymbolTableGen::visit(Decimal& n) {}
void SymbolTableGen::visit(String& n) {}

void SymbolTableGen::visit(FunctionCall& n)
{
    n.id_ref->accept(*this);
    n.arguments->accept(*this);
}

void SymbolTableGen::visit(BinaryOperator& n)
{
    n.lhs->accept(*this);
    n.rhs->accept(*this);
}

void SymbolTableGen::visit(Assignment& n)
{
    n.rhs->accept(*this);
}


void SymbolTableGen::visit(Return& n)
{
    n.expression->accept(*this);
}


void SymbolTableGen::visit(Record& n)
{
    n.group->accept(*this);
}

void SymbolTableGen::visit(TaggedUnion& n)
{
    for(auto variant : n.variants)
    {
        variant->accept(*this);
    }
}

void SymbolTableGen::visit(TaggedVariant& n) {}

void SymbolTableGen::visit(Variable& n)
{
    if(n.rhs != nullptr)
        n.rhs->accept(*this);
}

void SymbolTableGen::visit(LocalVariable& n)
{
    if(n.rhs != nullptr)
        n.rhs->accept(*this);
}

void SymbolTableGen::visit(GlobalVariable& n)
{
    if(n.rhs != nullptr)
        n.rhs->accept(*this);
}

void SymbolTableGen::visit(Function& n)
{
    for(auto param : n.params)
    {
        param->accept(*this);
    }
    
    n.statements->accept(*this);
}

void SymbolTableGen::visit(GlobalFunction& n)
{
    for(auto param : n.params)
    {
        param->accept(*this);
    }
    
    n.statements->accept(*this);
}

 void SymbolTableGen::visit(LocalFunction& n)
{
    for(auto param : n.params)
    {
        param->accept(*this);
    }
    
    n.statements->accept(*this);
}