#include "hkc/IdBuilder.h"

using namespace ast;

void IdBuilder::visit(ExpressionGroup& n)
{
    for(auto expr : n.exprs)
    {
        expr->accept(*this);
    }
}

void IdBuilder::visit(Source& n)
{
    for(auto module : n.modules)
    {
        namespace_stack.clear();
        module->accept(*this);
    }
}

void IdBuilder::visit(Module& n)
{
    push_namespace(n.id);
    n.group->accept(*this);
    pop_namespace();
}

void IdBuilder::visit(Submodule& n)
{
    prepend_namespace(n.id);
    push_namespace(n.id);
    
    n.group->accept(*this);
    
    pop_namespace();
}

void IdBuilder::visit(Import& n) {}
void IdBuilder::visit(QImport& n) {}
void IdBuilder::visit(IdentifierRef& n) {}
void IdBuilder::visit(Integer& n) {}
void IdBuilder::visit(Decimal& n) {}
void IdBuilder::visit(String& n) {}
void IdBuilder::visit(FunctionCall& n) {}
void IdBuilder::visit(BinaryOperator& n) {}
void IdBuilder::visit(Assignment& n) {}
void IdBuilder::visit(Return& n) {}


void IdBuilder::visit(Record& n)
{
    prepend_namespace(n.id);
    push_namespace(n.id);
    n.group->accept(*this);
    pop_namespace();
}

void IdBuilder::visit(TaggedUnion& n)
{
    prepend_namespace(n.id);
    push_namespace(n.id);
    for(auto variant : n.variants)
    {
        variant->accept(*this);
    }
    pop_namespace();
}

void IdBuilder::visit(TaggedVariant& n)
{
    prepend_namespace(n.tag);
}

void IdBuilder::visit(Variable& n)
{
    n.rhs->accept(*this);
}

void IdBuilder::visit(LocalVariable& n)
{
    if(n.rhs != nullptr)
        n.rhs->accept(*this);
}

void IdBuilder::visit(GlobalVariable& n)
{
    prepend_namespace(n.id);
}

void IdBuilder::visit(Function& n) {}

void IdBuilder::visit(GlobalFunction& n)
{
    prepend_namespace(n.id);
    
// Params don't need to be in a namespace
//     for(auto param : n.params)
//     {
//         param->accept(*this);
//     }
}

void IdBuilder::visit(LocalFunction& n) {}

void IdBuilder::push_namespace(std::string id)
{
    namespace_stack.push_back(id);
}

void IdBuilder::pop_namespace()
{
    namespace_stack.pop_back();
}

void IdBuilder::prepend_namespace(std::string& id)
{
    id.insert(0, namespace_stack.back() + '.');
}