#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

#include "hkc/ast.h"
#include "hkc/prefix_trie.h"
#include "hkc/suffix_trie.h"

#include <string>
#include <unordered_map>
#include <vector>

struct SymbolTable
{
    struct Scope {
        // List of symbols introduced by this scope
        std::vector<ast::IdentifierVec> symbols;
        
        Scope() : symbols() {}
    };
    
    ast::IdentifierVec curr_path;
    std::vector<Scope> scopes;
    SuffixTrie symbols;
    
    SymbolTable() : curr_path(), scopes(), symbols() {}
    
    void insert(ast::IdentifierVec qual_id, ast::Node* input);
    void clear();
    
    void enter_scope();
    void exit_scope();
    
    void import_prefix_trie(PrefixTrie trie);
};

#endif // _SYMBOL_TABLE_H_