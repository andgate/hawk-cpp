#ifndef _HKC_AST_H
#define _HKC_AST_H

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>


namespace ast
{
    class Source;
    class Module;
    class Submodule;
    class ModuleIdentifier;
    class Export;
    class QExport;
    class Import;
    class QImport;
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
    class Typedef;
    class GlobalTypedef;
    class LocalTypedef;
    class Variable;
    class GlobalVariable;
    class LocalVariable;
    class Function;
    class GlobalFunction;
    class LocalFunction;
    
    typedef std::string Identifier;
    
    typedef std::shared_ptr<Source> pSource;
    typedef std::shared_ptr<Module> pModule;
    typedef std::shared_ptr<Submodule> pSubmodule;
    typedef std::shared_ptr<ModuleIdentifier> pModuleIdentifier;
    typedef std::shared_ptr<Export> pExport;
    typedef std::shared_ptr<QExport> pQExport;
    typedef std::shared_ptr<Import> pImport;
    typedef std::shared_ptr<QImport> pQImport;
    typedef std::shared_ptr<Expression> pExpression;
    typedef std::shared_ptr<IdentifierRef> pIdentifierRef;
    typedef std::shared_ptr<NameBindings> pNameBindings;
    
    typedef std::shared_ptr<Typedef> pTypedef;
    typedef std::shared_ptr<GlobalTypedef> pGlobalTypedef;
    typedef std::shared_ptr<LocalTypedef> pLocalTypedef;
    
    typedef std::shared_ptr<Variable> pVariable;
    typedef std::shared_ptr<Variable> pGlobalVariable;
    typedef std::shared_ptr<Variable> pLocalVariable;
    
    typedef std::shared_ptr<Function> pFunction;
    typedef std::shared_ptr<Function> pGlobalFunction;
    typedef std::shared_ptr<Function> pLocalFunction;
    
    typedef std::vector<pModule> pModuleVec;
    typedef std::vector<pModuleIdentifier> pModuleIdentifierVec;
    typedef std::vector<Identifier> IdentifierVec;
    typedef std::vector<pExpression> pExpressionVec;
    typedef std::vector<pVariable> pVariableVec;
    
    pVariable mk_var(pNameBindings bindings, pExpression expr);
    pFunction mk_func(pNameBindings bindings, pExpressionVec exprs);
    
    pFunction promote_global(pFunction f);
    pVariable promote_global(pVariable v);
    
    pFunction promote_local(pFunction f);
    pVariable promote_local(pVariable v);
    
    pExport promote_qualified(pExport e);
    pImport promote_qualified(pImport i);

    class Visitor;

    struct Node
    {
        virtual ~Node() {}
        
        virtual void accept(Visitor& v) = 0;
    };

    struct Visitor
    {
        virtual void visit(Source& n) = 0;
        virtual void visit(Module& n) = 0;
        virtual void visit(Submodule& n) = 0;
        virtual void visit(Export& n) = 0;
        virtual void visit(QExport& n) = 0;
        virtual void visit(Import& n) = 0;
        virtual void visit(QImport& n) = 0;
        virtual void visit(IdentifierRef& n) = 0;
        virtual void visit(Integer& n) = 0;
        virtual void visit(Decimal& n) = 0;
        virtual void visit(String& n) = 0;
        virtual void visit(FunctionCall& n) = 0;
        virtual void visit(BinaryOperator& n) = 0;
        virtual void visit(Assignment& n) = 0;
        virtual void visit(Return& n) = 0;
        
        virtual void visit(Typedef& n) = 0;
        virtual void visit(GlobalTypedef& n) = 0;
        virtual void visit(LocalTypedef& n) = 0;
        
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
    
    struct Source : public Node
    {
        pModuleVec modules;
        
        Source()
        : modules() { }
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };

    struct Module : public Node
    {
        Identifier id;
        pExpressionVec exprs;
        
        Module(Identifier id, pExpressionVec exprs)
        : id(id), exprs(exprs) { }
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    struct Submodule : public Expression
    {
        Identifier id;
        pExpressionVec exprs;
        
        Submodule(Identifier id, pExpressionVec exprs)
        : id(id), exprs(exprs) { }
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    struct ModuleIdentifier
    {
        Identifier id;
        pModuleIdentifierVec subs;
        
        ModuleIdentifier()
        : id(), subs() {}
        ModuleIdentifier(Identifier id)
        : id(id), subs() {}
        ModuleIdentifier(Identifier id, pModuleIdentifierVec subs)
        : id(id), subs(subs) {}
    };
    
    
    struct Export : public Expression
    {
        pModuleIdentifier id;
        
        Export()
        : id() {}
        
        Export(pModuleIdentifier id)
        : id(id) {}
        
        Export(const Export& ex)
        : id(ex.id) {}
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    struct QExport : public Export
    {
        QExport() : Export() {}
        QExport(pModuleIdentifier id) : Export(id) {}
        QExport(const Export& ex)
        : Export(ex) {}
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    struct Import : public Expression
    {
        pModuleIdentifier id;
        
        Import()
        : id() {}
        
        Import(pModuleIdentifier id)
        : id(id) {}
        
        Import(const Import& import)
        : id(import.id) {}
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    struct QImport : public Import
    {
        QImport()
        : Import() {}
        
        QImport(pModuleIdentifier id)
        : Import(id) {}
        
        QImport(const Import& import)
        : Import(import) {}
        
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
        pExpressionVec arguments;
        
        FunctionCall(pIdentifierRef id_ref) : id_ref(id_ref), arguments() { }
        FunctionCall(pIdentifierRef id_ref, pExpressionVec arguments)
        : id_ref(id_ref), arguments(arguments) { }
        
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
    
    struct Typedef : public Expression 
    {
        Identifier id;
        pExpression rhs;
        
        Typedef(Identifier id, pExpression rhs)
        : id(id), rhs(rhs) { }
        
        Typedef(const Typedef& def)
        : id(def.id), rhs(def.rhs) {}
    };
    
    struct GlobalTypedef : public Typedef
    {
        GlobalTypedef(const Typedef& t) : Typedef(t) {}
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    struct LocalTypedef : public Typedef
    {
        LocalTypedef(const Typedef& t) : Typedef(t) {}
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
        pExpressionVec exprs;
        
        
        Function(Identifier id, Identifier type,
                            pVariableVec params,
                            pExpressionVec exprs)
        : attribs(), id(id), type(type)
        , params(params), exprs(exprs) {}
        
        Function(const Function& f)
        : attribs(f.attribs), id(f.id), type(f.type)
        , params(f.params), exprs(f.exprs) {}
        
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
        virtual void visit(Source& n)
        {
            for(auto module : n.modules)
            {
                module->accept(*this);
            }
        }
        
        virtual void visit(Module& n)
        {
            for(auto expr : n.exprs)
            {
                expr->accept(*this);
            }
        }
        
        virtual void visit(Submodule& n)
        {
            for(auto expr : n.exprs)
            {
                expr->accept(*this);
            }
        }
        
        virtual void visit(Export& n) {}
        virtual void visit(QExport& n) {}
        
        virtual void visit(Import& n) {}
        virtual void visit(QImport& n) {}
        
        virtual void visit(IdentifierRef& n) {}
        virtual void visit(Integer& n) {}
        virtual void visit(Decimal& n) {}
        virtual void visit(String& n) {}
        
        virtual void visit(FunctionCall& n)
        {
            n.id_ref->accept(*this);
            
            for(auto argument : n.arguments)
            {
                argument->accept(*this);
            }
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
        
        virtual void visit(Variable& n)
        {
            n.rhs->accept(*this);
        }
        
        virtual void visit(LocalVariable& n)
        {
            n.rhs->accept(*this);
        }
        
        virtual void visit(GlobalVariable& n)
        {
            n.rhs->accept(*this);
        }
        
        virtual void visit(Function& n)
        {
            for(auto param : n.params)
            {
                param->accept(*this);
            }
            
            for(auto expr : n.exprs)
            {
                expr->accept(*this);
            }
        }
        
        virtual void visit(GlobalFunction& n)
        {
            for(auto param : n.params)
            {
                param->accept(*this);
            }
            
            for(auto expr : n.exprs)
            {
                expr->accept(*this);
            }
        }
        
        virtual void visit(LocalFunction& n)
        {
            for(auto param : n.params)
            {
                param->accept(*this);
            }
            
            for(auto expr : n.exprs)
            {
                expr->accept(*this);
            }
        }
    };
}

#endif