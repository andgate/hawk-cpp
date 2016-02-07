#ifndef _HKC_AST_PRINTER_H_
#define _HKC_AST_PRINTER_H_

#include "hkc/ast.h"
#include <string>

namespace ast
{
    class Printer : public Visitor
    {
    public:
        Printer(): ws(), curr_indent(0) {}
        
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
        
    private:
        std::string ws;
        int curr_indent;
        
        void indent();
        void undent();
    };
}

#endif