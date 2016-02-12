#include "hkc/prefix_trie.h"


void PrefixTrie::insert(std::vector<std::string> id_path, ast::Node* input)
{
    PrefixTrie* curr_dict = this;
    size_t id_path_index = 0;
    
    while(id_path_index < id_path.size())
    {
        // Update the current id
        std::string curr_id = id_path[id_path_index];
        ++id_path_index;
        
        // Update the current_dict
        if(curr_dict->children.count(curr_id))
        {
            // If curr_id exists, update the curr_dict with it
            curr_dict = curr_dict->children[curr_id].get();
            if(id_path_index >= id_path.size())
            {
                std::string input_id = id_path[0];
                for(size_t i = 1; i < id_path.size(); ++i)
                    input_id += '.' + id_path[i];
                
                std::cout << "Warning: Identifier \"" << input_id << "\" was shadowed." << std::endl;
            }
        }
        else
        {
            // Else, create an new dict, insert it into the curr_dict
            // and set the new dict as current.
            auto new_dict = std::make_shared<PrefixTrie>(curr_id);
            curr_dict->children.insert({curr_id, new_dict});
            curr_dict = new_dict.get();
        }
        
        // If the path is exhausted, set the current dict's data as the input
        if(id_path_index >= id_path.size() && curr_id == curr_dict->id)
        {
            curr_dict->data = input;
        }
    }
}

pPrefixTrie PrefixTrie::find_sub(std::string id)
{
    return children[id];
}

pPrefixTrie PrefixTrie::find_sub(std::vector<std::string> id_path)
{
    PrefixTrie* curr_dict = this;
    size_t id_path_index = 0;
    
    while(id_path_index < id_path.size())
    {
        std::string curr_id = id_path[id_path_index];
        ++id_path_index;
        
        if(curr_dict->children.count(curr_id))
        {
            if(id_path_index >= id_path.size())
                return curr_dict->children[curr_id];
            
            curr_dict = curr_dict->children[curr_id].get();
        } else {
            std::string input_id = id_path[0];
            for(size_t i = 1; i < id_path.size(); ++i)
                input_id += '.' + id_path[i];
            
            std::cerr << "Warning: Id path \"" << input_id << "\" not found in dictionary." << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

void PrefixTrie::merge(PrefixTrie& dict)
{
    children.insert(dict.children.begin(), dict.children.end());
}

void PrefixTrie::merge(pPrefixTrie& dict)
{
    children.insert(dict->children.begin(), dict->children.end());
}

void PrefixTrie::print()
{
    print("");
}

void PrefixTrie::print(std::string prefix)
{
    if(!id.empty()) {
        std::cout << prefix << id << std::endl;    
        prefix += (id + ".");
    }
    
    for(auto child : children)
        child.second->print(prefix);
}