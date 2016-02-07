#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

#include <boost/algorithm/string.hpp>

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

template <class T>
struct Dictionary;

template <class T>
using pDictionary = std::shared_ptr<Dictionary<T>>;

template <class T>
struct Dictionary
{
    std::string id;
    T data;
    
    std::unordered_map<std::string, pDictionary<T>> children;
    
    Dictionary() : id(), data(), children() {}
    Dictionary(std::string id)
    : id(id), data(), children() {}
    Dictionary(std::string id, T data)
    : id(id), data(data), children() {}
    
    void insert(std::vector<std::string> id_path, T input)
    {
        Dictionary* curr_dict = this;
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
                    for(auto it = id_path.begin()+1; it != id_path.end(); ++it)
                        input_id += '.' + *it;
                    
                    std::cout << "Warning: Identifier \"" << input_id << "\" was shadowed." << std::endl;
                }
            }
            else
            {
                // Else, create an new dict, insert it into the curr_dict
                // and set the new dict as current.
                auto new_dict = std::make_shared<Dictionary<T>>(curr_id);
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
    
    void print()
    {
        std::cout << id << std::endl;
        
        for(auto child : children)
        {
            child.second->print("");
        }
    }
    
    void print(std::string prefix)
    {
        std::cout << prefix << id << std::endl;
        
        prefix += (id + ".");
        
        for(auto child : children)
        {
            child.second->print(prefix);
        }
    }
};

#endif // _DICTIONARY_H_