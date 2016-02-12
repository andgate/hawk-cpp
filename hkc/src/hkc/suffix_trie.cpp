#include "hkc/suffix_trie.h"

#include <iostream>
#include <utility>

void SuffixTrie::insert(std::vector<std::string> id_path, ast::Node* input)
{
    SuffixTrie* curr_node = this;
    
    for(auto curr_id = id_path.rbegin(); curr_id != id_path.rend(); ++curr_id)
    {
        if(curr_node->children.count(*curr_id)) {
            curr_node = curr_node->children[*curr_id].get();
        } else {
            curr_node->children.insert({*curr_id, std::make_shared<SuffixTrie>()});
        }
    }
    
    curr_node->data = input;
}

void SuffixTrie::clear()
{
    children.clear();
}

ast::Node* SuffixTrie::find(std::vector<std::string> id_path)
{
    SuffixTrie* curr_node = this;
    
    for(auto curr_id = id_path.rbegin(); curr_id != id_path.rend(); ++curr_id)
    {
        if(curr_node->children.count(*curr_id)) {
            curr_node = curr_node->children[*curr_id].get();
        }
        else {
            std::cout << "cannot find the id." << std::endl;
            return nullptr;
        }
    }
    
    return curr_node->data;
}

void SuffixTrie::remove(ast::IdentifierVec id_path)
{
    std::vector<std::pair<std::string, SuffixTrie*>> node_path;
    {
        auto curr_node = this;
        for(auto curr_id = id_path.rbegin(); curr_id != id_path.rend(); ++curr_id)
        {
            if(curr_node->children.count(*curr_id)) {
                node_path.push_back(std::make_pair(*curr_id, curr_node));
                curr_node = curr_node->children[*curr_id].get(); 
            }
            else {
                std::cout << "cannot find id." << std::endl;
                return;
            }
        }
    }
    
    for(uint i = node_path.size()-1; i >= 0; --i)
    {
        auto curr_node = node_path[i].second;
        if(curr_node->children.size() > 1)
        {
            curr_node->children.erase(node_path[i].first);
            return;
        }
    }
}