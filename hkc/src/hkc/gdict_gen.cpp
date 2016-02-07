#include "hkc/gdict_gen.h"

using namespace ast;

void ast::gen_gdict(pSource src)
{
    GDictGen gen;
    src->accept(gen);
}


void GDictGen::visit(Source& n)
{
    dict = std::make_shared<Dictionary<Node*>>("", &n);
    n.dict = dict;
    for(auto module : n.modules) module->accept(*this);
}

void GDictGen::visit(Module& n)
{
    id_path.insert(id_path.end(), n.id_path.begin(), n.id_path.end());
    dict->insert(id_path, &n);
    
    for(auto expr : n.exprs)
        expr->accept(*this);
    
    for(size_t i = 0; i < n.id_path.size(); ++i)
        id_path.pop_back();
}

void GDictGen::visit(Submodule& n)
{
    id_path.insert(id_path.end(), n.id_path.begin(), n.id_path.end());
    dict->insert(id_path, &n);
    
    for(auto expr : n.exprs)
        expr->accept(*this);
    
    for(size_t i = 0; i < n.id_path.size(); ++i)
        id_path.pop_back();
}

void GDictGen::visit(Import& n) {}
void GDictGen::visit(QImport& n) {}
void GDictGen::visit(IdentifierRef& n) {}
void GDictGen::visit(Integer& n) {}
void GDictGen::visit(Decimal& n) {}
void GDictGen::visit(String& n) {}
void GDictGen::visit(FunctionCall& n) {}
void GDictGen::visit(BinaryOperator& n) {}
void GDictGen::visit(Assignment& n) {}
void GDictGen::visit(Return& n) {}

void GDictGen::visit(Record& n)
{
    id_path.push_back(n.id);
    dict->insert(id_path, &n);
    
    for(auto expr : n.exprs)
        expr->accept(*this);
    
    id_path.pop_back();
}

void GDictGen::visit(TaggedUnion& n)
{
    id_path.push_back(n.id);
    dict->insert(id_path, &n);
    
    for(auto variant : n.variants)
        variant->accept(*this);
    
    id_path.pop_back();
}

void GDictGen::visit(TaggedVariant& n)
{
    id_path.push_back(n.tag);
    dict->insert(id_path, &n);
    id_path.pop_back();
}

void GDictGen::visit(Variable& n)
{
    id_path.push_back(n.id);
    dict->insert(id_path, &n);
    id_path.pop_back();
}

void GDictGen::visit(LocalVariable& n)
{
    id_path.push_back(n.id);
    dict->insert(id_path, &n);
    id_path.pop_back();
}

void GDictGen::visit(GlobalVariable& n)
{
    id_path.push_back(n.id);
    dict->insert(id_path, &n);
    id_path.pop_back();
}

void GDictGen::visit(Function& n)
{
    id_path.push_back(n.id);
    dict->insert(id_path, &n);
    id_path.pop_back();
}

void GDictGen::visit(GlobalFunction& n)
{
    id_path.push_back(n.id);
    dict->insert(id_path, &n);
    id_path.pop_back();
}

void GDictGen::visit(LocalFunction& n)
{
    id_path.push_back(n.id);
    dict->insert(id_path, &n);
    id_path.pop_back();
}