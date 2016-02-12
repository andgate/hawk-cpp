#include "hkc/global_symbol_collector.h"

using namespace ast;

pPrefixTrie ast::collect_global_symbols(pRootModule root)
{
    GlobalSymbolCollector collector;
    root->accept(collector);
    return collector.trie;
}


void GlobalSymbolCollector::visit(RootModule& n)
{
    trie = std::make_shared<PrefixTrie>("", &n);
    for(auto module : n.modules) module->accept(*this);
}

void GlobalSymbolCollector::visit(Module& n)
{
    id_path.insert(id_path.end(), n.id_path.begin(), n.id_path.end());
    trie->insert(id_path, &n);
    
    for(auto expr : n.exprs)
        expr->accept(*this);
    
    for(size_t i = 0; i < n.id_path.size(); ++i)
        id_path.pop_back();
}

void GlobalSymbolCollector::visit(Submodule& n)
{
    id_path.insert(id_path.end(), n.id_path.begin(), n.id_path.end());
    trie->insert(id_path, &n);
    
    for(auto expr : n.exprs)
        expr->accept(*this);
    
    for(size_t i = 0; i < n.id_path.size(); ++i)
        id_path.pop_back();
}

void GlobalSymbolCollector::visit(Import& n) {}
void GlobalSymbolCollector::visit(IdentifierRef& n) {}
void GlobalSymbolCollector::visit(Integer& n) {}
void GlobalSymbolCollector::visit(Decimal& n) {}
void GlobalSymbolCollector::visit(String& n) {}
void GlobalSymbolCollector::visit(FunctionCall& n) {}
void GlobalSymbolCollector::visit(BinaryOperator& n) {}
void GlobalSymbolCollector::visit(Assignment& n) {}
void GlobalSymbolCollector::visit(Return& n) {}

void GlobalSymbolCollector::visit(Record& n)
{
    id_path.push_back(n.id);
    trie->insert(id_path, &n);
    
    for(auto expr : n.exprs)
        expr->accept(*this);
    
    id_path.pop_back();
}

void GlobalSymbolCollector::visit(TaggedUnion& n)
{
    id_path.push_back(n.id);
    trie->insert(id_path, &n);
    
    for(auto variant : n.variants)
        variant->accept(*this);
    
    id_path.pop_back();
}

void GlobalSymbolCollector::visit(TaggedVariant& n)
{
    id_path.push_back(n.tag);
    trie->insert(id_path, &n);
    id_path.pop_back();
}

void GlobalSymbolCollector::visit(Variable& n)
{
    id_path.push_back(n.id);
    trie->insert(id_path, &n);
    id_path.pop_back();
}

void GlobalSymbolCollector::visit(LocalVariable& n)
{
    id_path.push_back(n.id);
    trie->insert(id_path, &n);
    id_path.pop_back();
}

void GlobalSymbolCollector::visit(GlobalVariable& n)
{
    id_path.push_back(n.id);
    trie->insert(id_path, &n);
    id_path.pop_back();
}

void GlobalSymbolCollector::visit(Function& n)
{
    id_path.push_back(n.id);
    trie->insert(id_path, &n);
    id_path.pop_back();
}

void GlobalSymbolCollector::visit(GlobalFunction& n)
{
    id_path.push_back(n.id);
    trie->insert(id_path, &n);
    id_path.pop_back();
}

void GlobalSymbolCollector::visit(LocalFunction& n)
{
    id_path.push_back(n.id);
    trie->insert(id_path, &n);
    id_path.pop_back();
}