#include "hkc/symtab.h"

void SymbolTable::insert(ast::IdentifierVec qual_id, ast::Expression* input)
{
    scopes.back().symbols.push_back(qual_id);
    symbols.insert(qual_id, input);
}

void SymbolTable::clear()
{
    curr_path.clear();
    scopes.clear();
    symbols.clear();
}

void SymbolTable::enter_scope()
{
    // Push a new scope onto the stack
    scopes.push_back(Scope());
    // That's about it.
}

void SymbolTable::exit_scope()
{
    for(auto symbol : scopes.back().symbols)
    {
        // Remove the symbols in scope
        symbols.remove(symbol);
    }
    
    scopes.pop_back();
}

void import_prefix_trie(PrefixTrie trie)
{
}