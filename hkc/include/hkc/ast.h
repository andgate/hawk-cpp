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
    class Variable;
    class LocalVariable;
    class GlobalVariable;
    class Function;
    class LocalFunction;
    class GlobalFunction;
    
    typedef std::string Identifier;
    
    typedef std::shared_ptr<Source> pSource;
    typedef std::shared_ptr<Module> pModule;
    typedef std::shared_ptr<Expression> pExpression;
    typedef std::shared_ptr<IdentifierRef> pIdentifierRef;
    typedef std::shared_ptr<NameBindings> pNameBindings;
    typedef std::shared_ptr<Variable> pVariable;
    typedef std::shared_ptr<Variable> pLocalVariable;
    typedef std::shared_ptr<Variable> pGlobalVariable;
    typedef std::shared_ptr<Function> pFunction;
    typedef std::shared_ptr<Function> pLocalFunction;
    typedef std::shared_ptr<Function> pGlobalFunction;
    
    typedef std::vector<pModule> pModuleVec;
    typedef std::vector<Identifier> IdentifierVec;
    typedef std::vector<pExpression> pExpressionVec;
    typedef std::vector<pVariable> pVariableVec;
    
    pVariable mk_var(pNameBindings bindings, pExpression expr);
    pFunction mk_func(pNameBindings bindings, pExpressionVec exprs);
    
    pGlobalFunction promote_global(pFunction f);
    pGlobalVariable promote_global(pVariable v);
    
    pLocalFunction promote_local(pFunction f);
    pLocalVariable promote_local(pVariable v);

    class Visitor;

    class Node {
    public:
        virtual ~Node() {}
        
        virtual void accept(Visitor& v) = 0;
    };

    class Visitor
    {
    public:
        virtual void visit(Source& n) = 0;
        virtual void visit(Module& n) = 0;
        virtual void visit(IdentifierRef& n) = 0;
        virtual void visit(Integer& n) = 0;
        virtual void visit(Decimal& n) = 0;
        virtual void visit(String& n) = 0;
        virtual void visit(FunctionCall& n) = 0;
        virtual void visit(BinaryOperator& n) = 0;
        virtual void visit(Assignment& n) = 0;
        virtual void visit(Return& n) = 0;
        virtual void visit(Variable& n) = 0;
        virtual void visit(GlobalVariable& n) = 0;
        virtual void visit(LocalVariable& n) = 0;
        virtual void visit(Function& n) = 0;
        virtual void visit(GlobalFunction& n) = 0;
        virtual void visit(LocalFunction& n) = 0;
    };

    class Expression : public Node {
    public:
        virtual void accept(Visitor &v) {}
    };
    
    class Source : public Node
    {
    public:
        pModuleVec modules;
        
        Source()
        : modules() { }
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };

    class Module : public Node
    {
    public:
        Identifier id;
        pExpressionVec exprs;
        
        Module(Identifier id, pExpressionVec exprs)
        : id(id), exprs(exprs) { }
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    class IdentifierRef : public Expression {
    public:
        Identifier str;
        IdentifierRef(const Identifier& str) : str(str) { }
        IdentifierRef(IdentifierRef& ident) : str(ident.str) {}
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };

    class Integer : public Expression {
    public:
        long long int value;
        Integer(long long int value) : value(value) { }
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };

    class Decimal : public Expression {
    public:
        long double value;
        Decimal(long double value) : value(value) { }
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };

    class String : public Expression {
    public:
        std::string value;
        String(const std::string& value) : value(value) {}
        
        virtual void accept(Visitor &v) {v.visit(*this); }
    };

    class FunctionCall : public Expression {
    public:
        pIdentifierRef id_ref;
        pExpressionVec arguments;
        
        FunctionCall(pIdentifierRef id_ref) : id_ref(id_ref), arguments() { }
        FunctionCall(pIdentifierRef id_ref, pExpressionVec arguments)
        : id_ref(id_ref), arguments(arguments) { }
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };

    class BinaryOperator : public Expression {
    public:
        pExpression lhs;
        std::string op;
        pExpression rhs;
        
        BinaryOperator(pExpression lhs, std::string op, pExpression rhs)
        : lhs(lhs), op(op), rhs(rhs)
        { }
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };

    class Assignment : public Expression {
    public:
        pIdentifierRef lhs;
        pExpression rhs;
        Assignment(pIdentifierRef lhs, pExpression rhs) : 
        lhs(lhs), rhs(rhs) { }

        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    class Return : public Expression {
    public:
        pExpression expression;
        
        Return(pExpression expression)
        : expression(expression) { }
        
        virtual void accept(Visitor &v) { v.visit(*this); }
    };

    class NameBindings {
    public:
        IdentifierVec ids;
        IdentifierVec typeids;
        
        NameBindings(IdentifierVec ids, IdentifierVec typeids)
        : ids(ids), typeids(typeids) {}
    };

    class Variable : public Expression {
    public:
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
    
    class LocalVariable : public Variable
    {
    public:
        LocalVariable(const Variable& v) : Variable(v) {}
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    class GlobalVariable : public Variable
    {
    public:
        GlobalVariable(const Variable& v) : Variable(v) {}
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    

    class Function : public Expression {
    public:
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
    
    class LocalFunction : public Function
    {
    public:
        LocalFunction(const Function& f): Function(f) {}
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    class GlobalFunction : public Function
    {
    public:
        GlobalFunction(const Function& f): Function(f) {}
        virtual void accept(Visitor &v) { v.visit(*this); }
    };
    
    
    class VisitorAdapter : public Visitor
    {
    public:
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