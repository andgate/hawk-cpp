#ifndef _SYMBOL_TABLE_H_
#define _SYMBOL_TABLE_H_

#include <string>
#include <unordered_map>

namespace ast
{
    class Module;
    class Function;
    class Variable;
    class Record;
    class TaggedUnion;
}

class SymbolTable
{
    std::unordered_map<std::string, ast::Module*> modules;
    std::unordered_map<std::string, ast::Function*> functions;
    std::unordered_map<std::string, ast::Variable*> variables;
    std::unordered_map<std::string, ast::Record*> record;
    std::unordered_map<std::string, ast::TaggedUnion*> unions;
    
public:
    SymbolTable() : functions(),  {}
};

#endif // _SYMBOL_TABLE_H_