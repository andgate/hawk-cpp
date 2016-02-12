#include "hkc/ast.h"

using namespace ast;

std::string ast::mk_id(IdentifierVec& id_path)
{
    std::string id;
    if(id_path.empty()) return id;
    
    id += id_path[0];
    for(size_t i = 1; i < id_path.size(); ++i)
        id += '.' + id_path[i];
    return id;
}

IdPathVec ast::mk_mod_id(pModuleIdentifier mod)
{
    IdPathVec id_paths;
    
    if(mod->subs.empty())
    {
        id_paths.push_back(mod->id_path);
    } else
    {
        for(auto sub : mod->subs)
        {
            auto sub_paths(mk_mod_id(sub));
            for(auto sub_path : sub_paths)
            {
                IdentifierVec curr_path;
                curr_path.insert(curr_path.end(), mod->id_path.begin(), mod->id_path.end());
                curr_path.insert(curr_path.end(), sub_path.begin(), sub_path.end());
                id_paths.push_back(curr_path);
            }
        }
    }
    
    return id_paths;
}

pVariable ast::mk_var(pNameBindings bindings, pExpression expr)
{
    // Sanity Check
    Identifier var_id; 
    if(!bindings->ids.empty())
    {
        var_id = bindings->ids[0];
    } else
    {
        std::cerr << "Incomplete variable declaration! Missing name!" << std::endl;
        exit(1);
    }
    
    Identifier type_id;
    if(!bindings->typeids.empty())
    {
        type_id = bindings->typeids.back();
    }
    
    return std::make_shared<Variable>(var_id, type_id, expr);
}

pFunction ast::mk_func(pNameBindings bindings, pExpressionVec stmts)
{
    Identifier func_id = bindings->ids.front();
    uint param_id_count;
    if(!bindings->ids.empty())
    {
        func_id = bindings->ids.front();
        param_id_count = bindings->ids.size() - 1;
    }
    
    Identifier func_type;
    uint param_type_count;
    if(!bindings->typeids.empty())
    {
        func_type = bindings->typeids.back();
        param_type_count = bindings->typeids.size() - 1;
    }
    
    
    pVariableVec params;
    auto param_count = std::max(param_id_count, param_type_count); 
    for(uint i = 0; i < param_count; i++)
    {
        Identifier param_name;
        if(i < param_id_count)
        {
            param_name = bindings->ids[i+1];
        }
        
        Identifier param_type;
        if(i < param_type_count)
        {
            param_type = bindings->typeids[i];
        }
        
        params.push_back(std::make_shared<Variable>(param_name, param_type));
    }
    
    return std::make_shared<Function>(func_id, func_type, params, stmts);
}


pFunction ast::promote_global(pFunction f)
{
    return std::make_shared<GlobalFunction>(*f);
}

pVariable ast::promote_global(pVariable v)
{
    return std::make_shared<GlobalVariable>(*v);
}


pFunction ast::promote_local(pFunction f)
{
    return std::make_shared<LocalFunction>(*f);
}

pVariable ast::promote_local(pVariable v)
{
    return std::make_shared<LocalVariable>(*v);
}