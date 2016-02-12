#ifndef _HKC_AST_H
#define _HKC_AST_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace ast
{
    class Node;
    class RootModule;
    class Module;
    class Submodule;
    class ModuleIdentifier;
    class Import;
    class Expression;
    class IdentifierRef;
    class Integer;
    class Decimal;
    class String;
    class FunctionCall;
    class BinaryOperator;
    class Assignment;
    class Return;
    class NameBindings;
    class Record;
    class TaggedUnion;
    class TaggedVariant;
    class Variable;
    class GlobalVariable;
    class LocalVariable;
    class Function;
    class GlobalFunction;
    class LocalFunction;
    
    typedef std::string Identifier;
    
    typedef std::shared_ptr<Node> pNode;
    typedef std::shared_ptr<RootModule> pRootModule;
    typedef std::shared_ptr<Module> pModule;
    typedef std::shared_ptr<Submodule> pSubmodule;
    typedef std::shared_ptr<ModuleIdentifier> pModuleIdentifier;
    typedef std::shared_ptr<Import> pImport;
    typedef std::shared_ptr<Expression> pExpression;
    typedef std::shared_ptr<IdentifierRef> pIdentifierRef;
    typedef std::shared_ptr<NameBindings> pNameBindings;
    
    typedef std::shared_ptr<Record> pRecord;
    typedef std::shared_ptr<TaggedUnion> pTaggedUnion;
    typedef std::shared_ptr<TaggedVariant> pTaggedVariant;
    
    typedef std::shared_ptr<Variable> pVariable;
    typedef std::shared_ptr<Variable> pGlobalVariable;
    typedef std::shared_ptr<Variable> pLocalVariable;
    
    typedef std::shared_ptr<Function> pFunction;
    typedef std::shared_ptr<Function> pGlobalFunction;
    typedef std::shared_ptr<Function> pLocalFunction;
    
    typedef std::vector<pModule> pModuleVec;
    typedef std::vector<pModuleIdentifier> pModuleIdentifierVec;
    typedef std::vector<Identifier> IdentifierVec;
    typedef std::vector<IdentifierVec> IdPathVec;
    typedef std::vector<pTaggedUnion> pTaggedUnionVec;
    typedef std::vector<pTaggedVariant> pTaggedVariantVec;
    typedef std::vector<pExpression> pExpressionVec;
    typedef std::vector<pVariable> pVariableVec;
    
    std::string mk_id(IdentifierVec& id_path);
    
    IdPathVec mk_mod_id(pModuleIdentifierVec mods);
    IdPathVec mk_mod_id(pModuleIdentifier mod);
    
    pVariable mk_var(pNameBindings bindings, pExpression expr);
    pFunction mk_func(pNameBindings bindings, pExpressionVec statements);
    
    pFunction promote_global(pFunction f);
    pVariable promote_global(pVariable v);
    
    pFunction promote_local(pFunction f);
    pVariable promote_local(pVariable v);
    
    pImport promote_public(pImport i);

    class Visitor;

    struct Node
    {
        virtual ~Node() {}
        
        virtual void accept(Visitor& v) = 0;
    };

    struct Visitor
    {
        virtual ~Visitor() {}
        
        virtual void visit(RootModule& n) = 0;
        virtual void visit(Module& n) = 0;
        virtual void visit(Submodule& n) = 0;
        virtual void visit(Import& n) = 0;
        virtual void visit(IdentifierRef& n) = 0;
        virtual void visit(Integer& n) = 0;
        virtual void visit(Decimal& n) = 0;
        virtual void visit(String& n) = 0;
        virtual void visit(FunctionCall& n) = 0;
        virtual void visit(BinaryOperator& n) = 0;
        virtual void visit(Assignment& n) = 0;
        virtual void visit(Return& n) = 0;
        
        virtual void visit(Record& n) = 0;
        virtual void visit(TaggedUnion& n) = 0;
        virtual void visit(TaggedVariant& n) = 0;
        
        virtual void visit(Variable& n) = 0;
        virtual void visit(GlobalVariable& n) = 0;
        virtual void visit(LocalVariable& n) = 0;
        
        virtual void visit(Function& n) = 0;
        virtual void visit(GlobalFunction& n) = 0;
        virtual void visit(LocalFunction& n) = 0;
    };

    struct Expression : public Node
    {
        virtual void accept(Visitor &v) {}
    };
    
    struct RootModule : public Node
    {
        pModuleVec modules;
        
        RootModule()
        : modules() { }
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };

    struct Module : public Node
    {
        IdentifierVec id_path;
        pExpressionVec exprs;
        
        Module(IdentifierVec id_path)
        : id_path(id_path), exprs() { }
        Module(IdentifierVec id_path, pExpressionVec exprs)
        : id_path(id_path), exprs(exprs) { }
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    struct Submodule : public Expression
    {   
        IdentifierVec id_path;
        pExpressionVec exprs;
        
        Submodule(IdentifierVec id_path, pExpressionVec exprs)
        : id_path(id_path), exprs(exprs) { }
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    struct ModuleIdentifier
    {
        IdentifierVec id_path;
        pModuleIdentifierVec subs;
        
        ModuleIdentifier()
        : id_path(), subs() {}
        ModuleIdentifier(IdentifierVec id_path)
        : id_path(id_path), subs() {}
        ModuleIdentifier(IdentifierVec id_path, pModuleIdentifierVec subs)
        : id_path(id_path), subs(subs) {}
    };
    
    struct Import : public Expression
    {
        std::vector<IdentifierVec> id_paths;
        std::string alias_id;
        bool is_public;
        
        Import()
        : id_paths(), alias_id(), is_public(false) {}
        
        Import(std::vector<IdentifierVec> id_paths)
        : id_paths(id_paths), alias_id(), is_public(false) {}
        
        Import(std::vector<IdentifierVec> id_paths, std::string alias_id, bool is_public)
        : id_paths(id_paths), alias_id(alias_id), is_public(is_public) {}
        
        Import(const Import& import)
        : id_paths(import.id_paths), alias_id(import.alias_id), is_public(import.is_public) {}
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    struct IdentifierRef : public Expression
    {
        Identifier str;
        IdentifierRef(const Identifier& str) : str(str) { }
        IdentifierRef(IdentifierRef& ident) : str(ident.str) {}
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };

    struct Integer : public Expression
    {
        long long int value;
        Integer(long long int value) : value(value) { }
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };

    struct Decimal : public Expression
    {
        long double value;
        Decimal(long double value) : value(value) { }
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };

    struct String : public Expression
    {
        std::string value;
        String(const std::string& value) : value(value) {}
        
        virtual void accept(Visitor &v) {v.visit(*this); }
    };

    struct FunctionCall : public Expression
    {
        pIdentifierRef id_ref;
        pExpressionVec args;
        
        FunctionCall(pIdentifierRef id_ref) : id_ref(id_ref), args() { }
        FunctionCall(pIdentifierRef id_ref, pExpressionVec arguments)
        : id_ref(id_ref), args(arguments) { }
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };

    struct BinaryOperator : public Expression
    {
        pExpression lhs;
        std::string op;
        pExpression rhs;
        
        BinaryOperator(pExpression lhs, std::string op, pExpression rhs)
        : lhs(lhs), op(op), rhs(rhs)
        { }
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };

    struct Assignment : public Expression
    {
        pIdentifierRef lhs;
        pExpression rhs;
        Assignment(pIdentifierRef lhs, pExpression rhs) : 
        lhs(lhs), rhs(rhs) { }

        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    struct Return : public Expression
    {
        pExpression expression;
        
        Return(pExpression expression)
        : expression(expression) { }
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };

    struct NameBindings
    {
        IdentifierVec ids;
        IdentifierVec typeids;
        
        NameBindings(IdentifierVec ids, IdentifierVec typeids)
        : ids(ids), typeids(typeids) {}
    };
    
    struct Record : public Expression
    {
        Identifier id;
        pExpressionVec exprs;
        
        Record(Identifier id, pExpressionVec exprs)
        : id(id), exprs(exprs) { }
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    struct TaggedUnion : public Expression
    {
        Identifier id;
        pTaggedVariantVec variants;
        
        TaggedUnion(Identifier id, pTaggedVariantVec variants)
        : id(id), variants(variants) {}
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    struct TaggedVariant : public Expression
    {
        Identifier tag;
        IdentifierVec types;
        
        TaggedVariant(Identifier tag, IdentifierVec types)
        : tag(tag), types(types) {}
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    
    struct Variable : public Expression
    {
        IdentifierVec attribs;
        Identifier id;
        Identifier type;
        pExpression rhs;
        
        Variable(Identifier id, Identifier type)
        : attribs(), id(id), type(type), rhs(nullptr) { }
        Variable(Identifier id, Identifier type, pExpression rhs)
        : attribs(), id(id), type(type), rhs(rhs) { }
        
        Variable(const Variable& v)
        : attribs(v.attribs), id(v.id), type(v.type), rhs(v.rhs) { }
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    struct GlobalVariable : public Variable
    {
        GlobalVariable(const Variable& v) : Variable(v) {}
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    struct LocalVariable : public Variable
    {
        LocalVariable(const Variable& v) : Variable(v) {}
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    

    struct Function : public Expression
    {
        IdentifierVec attribs;
        Identifier id;
        Identifier type;
        pVariableVec params;
        pExpressionVec stmts;
        
        
        Function(Identifier id, Identifier type,
                            pVariableVec params,
                 pExpressionVec statements)
        : attribs(), id(id), type(type)
        , params(params), stmts(statements) {}
        
        Function(const Function& f)
        : attribs(f.attribs), id(f.id), type(f.type)
        , params(f.params), stmts(f.stmts) {}
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    struct GlobalFunction : public Function
    {
        GlobalFunction(const Function& f): Function(f) {}
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    struct LocalFunction : public Function
    {
        LocalFunction(const Function& f): Function(f) {}
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    
    
    struct VisitorAdapter : public Visitor
    {
        virtual void visit(RootModule& n)
        {
            for(auto module : n.modules)
            {
                module->accept(*this);
            }
        }
        
        virtual void visit(Module& n)
        {
            for(auto expr : n.exprs)
                expr->accept(*this);
        }
        
        virtual void visit(Submodule& n)
        {
            for(auto expr : n.exprs)
                expr->accept(*this);
        }
        
        virtual void visit(Import& n) {}
        
        virtual void visit(IdentifierRef& n) {}
        virtual void visit(Integer& n) {}
        virtual void visit(Decimal& n) {}
        virtual void visit(String& n) {}
        
        virtual void visit(FunctionCall& n)
        {
            n.id_ref->accept(*this);
            for(auto arg : n.args)
                arg->accept(*this);
        }
        
        virtual void visit(BinaryOperator& n)
        {
            n.lhs->accept(*this);
            n.rhs->accept(*this);
        }
        
        virtual void visit(Assignment& n)
        {
            n.rhs->accept(*this);
        }
        
        
        virtual void visit(Return& n)
        {
            n.expression->accept(*this);
        }
        
        
        virtual void visit(Record& n)
        {
            for(auto expr : n.exprs)
                expr->accept(*this);
        }
        
        virtual void visit(TaggedUnion& n)
        {
            for(auto variant : n.variants)
            {
                variant->accept(*this);
            }
        }
        
        virtual void visit(TaggedVariant& n) {}
        
        virtual void visit(Variable& n)
        {
            if(n.rhs != nullptr)
                n.rhs->accept(*this);
        }
        
        virtual void visit(LocalVariable& n)
        {
            if(n.rhs != nullptr)
                n.rhs->accept(*this);
        }
        
        virtual void visit(GlobalVariable& n)
        {
            if(n.rhs != nullptr)
                n.rhs->accept(*this);
        }
        
        virtual void visit(Function& n)
        {
            for(auto param : n.params)
            {
                param->accept(*this);
            }
            
            for(auto stmt : n.stmts)
                stmt->accept(*this);
        }
        
        virtual void visit(GlobalFunction& n)
        {
            for(auto param : n.params)
            {
                param->accept(*this);
            }
            
            for(auto stmt : n.stmts)
                stmt->accept(*this);
        }
        
        virtual void visit(LocalFunction& n)
        {
            for(auto param : n.params)
            {
                param->accept(*this);
            }
            
            for(auto stmt : n.stmts)
                stmt->accept(*this);
        }
    };
}

#endif