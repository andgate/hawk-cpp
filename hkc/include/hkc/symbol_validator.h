#ifndef _SYMBOL_VALIDATOR_H_
#define _SYMBOL_VALIDATOR_H_

#include "hkc/ast.h"
#include "hkc/prefix_trie.h"
#include "hkc/symtab.h"

#include <string>
#include <unordered_map>

namespace ast
{
    void validate_symbols(pModule root);
    
    struct SymbolValidator : public Visitor
    {
        SymbolTable symtab;
        IdentifierVec id_path;
        
        SymbolValidator()
        : symtab(), id_path() {}
        
        void visit(Module& n) override;
        
        void visit(Import& n) override;
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

#endif // _SYMBOL_VALIDATOR_H_