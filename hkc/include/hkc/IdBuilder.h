#ifndef _ID_BUILDER_H_
#define _ID_BUILDER_H_

#include "hkc/ast.h"
#include <vector>

namespace ast
{

    class IdBuilder : public Visitor
    {
        std::vector<std::string> namespace_stack;
        
        void push_namespace(std::string id);
        void pop_namespace();
        void prepend_namespace(std::string& id);
        
    public:
        IdBuilder(): namespace_stack() {}
        
        void visit(ExpressionGroup& n) override;
        
        void visit(Source& n) override;
        void visit(Module& n) override;
        void visit(Submodule& n) override;
        
        void visit(Import& n) override;
        void visit(QImport& n) override;
        
        void visit(IdentifierRef& n) override;
        void visit(Integer& n) override;
        void visit(Decimal& n) override;
        void visit(String& n) override;
        void visit(FunctionCall& n) override;
        void visit(BinaryOperator& n) override;
        void visit(Assignment& n) override;
        void visit(Return& n) override;
        
        void visit(Record& n) override;
        void visit(TaggedUnion& n) override;
        void visit(TaggedVariant& n) override;
        
        void visit(Variable& n) override;
        void visit(GlobalVariable& n) override;
        void visit(LocalVariable& n) override;
        
        void visit(Function& n) override;
        void visit(GlobalFunction& n) override;
        void visit(LocalFunction& n) override;
    };

}

#endif // _ID_BUILDER_H_