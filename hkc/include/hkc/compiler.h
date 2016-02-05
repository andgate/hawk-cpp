#ifndef HKC_COMPILER_H
#define HKC_COMPILER_H

#include "hkc/build.h"
#include "hkc/driver.h"
#include "hkc/ast.h"

#include <string>
#include <memory>

namespace hkc
{
    class Compiler
    {
    private:
        Build_ptr build;
        ast::pSource src_ast;
        
    public:
        Compiler(Build_ptr build)
        : build(build), src_ast(std::make_shared<ast::Source>()) {}
        
        void run();
        
        void parse();
        void process();
        
        void produce_output();
        
        void gen_ir(ast::Module& module, const std::string& out_file);
        void gen_asm(const std::string& in_file, const std::string& out_file);
        void gen_obj(const std::string& in_file, const std::string& out_file);
        
        void link(const std::vector<std::string>& in_files);
    };
}

#endif
