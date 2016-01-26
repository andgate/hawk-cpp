#ifndef _HKC_AST_PRINTER_H_
#define _HKC_AST_PRINTER_H_

#include "hkc/ast.h"

namespace ast
{
    class Printer : public Visitor
    {
    public:    
        void visit(Source& n);
        void visit(Module& n);
        void visit(IdentifierRef& n);
        void visit(Integer& n);
        void visit(Decimal& n);
        void visit(String& n);
        void visit(FunctionCall& n);
        void visit(BinaryOperator& n);
        void visit(Assignment& n);
        void visit(Return& n);
        void visit(Variable& n);
        void visit(Function& n);
    };
}

#endif