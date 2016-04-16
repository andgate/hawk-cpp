#ifndef _SUFFIX_TRIE_H_
#define _SUFFIX_TRIE_H_

#include "hkc/ast.h"

#include <memory>
#include <string>
#include <unordered_map>

struct SuffixTrie;
typedef std::shared_ptr<SuffixTrie> pSuffixTrie;

struct SuffixTrie
{
    ast::Expression* data;
    std::unordered_map<std::string, pSuffixTrie> children;
    
    SuffixTrie(): data(), children() {}
    
    void insert(std::vector<std::string> id_path, ast::Expression* input);
    void clear();
    ast::Expression* find(std::vector<std::string> id_path);
    void remove(ast::IdentifierVec id_path);
    
    /*void print();*/
    
};

#endif // _SUFFIX_TRIE_H_