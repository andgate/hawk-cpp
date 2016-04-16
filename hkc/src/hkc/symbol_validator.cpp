#include "hkc/symbol_validator.h"

using namespace ast;

void ast::validate_symbols(pRootModule root)
{
    SymbolValidator validator;
    root->accept(validator);
}


void SymbolValidator::visit(RootModule& n)
{
    for(auto module : n.modules)
    {
        symtab.clear();
        module->accept(*this);
    }
}

void SymbolValidator::visit(Module& n)
{
    id_path.insert(id_path.end(), n.id_path.begin(), n.id_path.end());
    symtab.insert(id_path, &n);
    
    for(auto expr : n.exprs)
        expr->accept(*this);
    
    for(size_t i = 0; i < n.id_path.size(); ++i)
        id_path.pop_back();
}

void SymbolValidator::visit(Submodule& n)
{
    id_path.insert(id_path.end(), n.id_path.begin(), n.id_path.end());
    symtab.insert(id_path, &n);
    
    for(auto expr : n.exprs)
        expr->accept(*this);
    
    for(size_t i = 0; i < n.id_path.size(); ++i)
        id_path.pop_back();
}

void SymbolValidator::visit(Import& n) {}
void SymbolValidator::visit(IdentifierRef& n) {}
void SymbolValidator::visit(Integer& n) {}
void SymbolValidator::visit(Decimal& n) {}
void SymbolValidator::visit(String& n) {}
void SymbolValidator::visit(FunctionCall& n) {}
void SymbolValidator::visit(BinaryOperator& n) {}
void SymbolValidator::visit(Assignment& n) {}
void SymbolValidator::visit(Return& n) {}

void SymbolValidator::visit(Record& n)
{
    id_path.push_back(n.id);
    symtab.insert(id_path, &n);
    
    for(auto expr : n.exprs)
        expr->accept(*this);
    
    id_path.pop_back();
}

void SymbolValidator::visit(TaggedUnion& n)
{
    id_path.push_back(n.id);
    symtab.insert(id_path, &n);
    
    for(auto variant : n.variants)
        variant->accept(*this);
    
    id_path.pop_back();
}

void SymbolValidator::visit(TaggedVariant& n)
{
    id_path.push_back(n.tag);
    symtab.insert(id_path, &n);
    id_path.pop_back();
}

void SymbolValidator::visit(Variable& n)
{
    id_path.push_back(n.id);
    symtab.insert(id_path, &n);
    id_path.pop_back();
}

void SymbolValidator::visit(LocalVariable& n)
{
    id_path.push_back(n.id);
    symtab.insert(id_path, &n);
    id_path.pop_back();
}

void SymbolValidator::visit(GlobalVariable& n)
{
    id_path.push_back(n.id);
    symtab.insert(id_path, &n);
    id_path.pop_back();
}

void SymbolValidator::visit(Function& n)
{
    id_path.push_back(n.id);
    symtab.insert(id_path, &n);
    id_path.pop_back();
}

void SymbolValidator::visit(GlobalFunction& n)
{
    id_path.push_back(n.id);
    symtab.insert(id_path, &n);
    id_path.pop_back();
}

void SymbolValidator::visit(LocalFunction& n)
{
    id_path.push_back(n.id);
    symtab.insert(id_path, &n);
    id_path.pop_back();
}