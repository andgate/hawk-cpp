#ifndef _PREFIX_TRIE_H_
#define _PREFIX_TRIE_H_

#include "hkc/ast.h"

#include <boost/algorithm/string.hpp>

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

struct PrefixTrie;
typedef std::shared_ptr<PrefixTrie> pPrefixTrie;
typedef std::unordered_map<std::string, pPrefixTrie> PrefixTrieMap;

struct PrefixTrie
{
    std::string id;
    ast::Node* data;
    PrefixTrieMap children;
    
    PrefixTrie() : id(), data(), children() {}
    PrefixTrie(std::string id)
    : id(id), data(), children() {}
    PrefixTrie(std::string id, ast::Node* data)
    : id(id), data(data), children() {}
    
    void insert(std::vector<std::string> id_path, ast::Node* input);
    void merge(PrefixTrie& sub);
    void merge(pPrefixTrie& sub);
    pPrefixTrie find_sub(std::string id);
    pPrefixTrie find_sub(std::vector<std::string> id_path);
    
    void print();
    void print(std::string prefix);
    
};



#endif // _PREFIX_TRIE_H_