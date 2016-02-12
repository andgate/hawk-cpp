#ifndef _GLOBAL_SYMBOL_COLLECTOR_GEN_H_
#define _GLOBAL_SYMBOL_COLLECTOR_GEN_H_

#include "hkc/ast.h"
#include "hkc/prefix_trie.h"

#include <string>
#include <unordered_map>

namespace ast
{
    pPrefixTrie collect_global_symbols(pRootModule root);
    
    struct GlobalSymbolCollector : public Visitor
    {
        pPrefixTrie trie;
        IdentifierVec id_path;

        GlobalSymbolCollector()
        : trie(), id_path() {}
        
        void visit(RootModule& n) override;
        void visit(Module& n) override;
        void visit(Submodule& n) override;
        
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

#endif // _GLOBAL_SYMBOL_COLLECTOR_GEN_H_