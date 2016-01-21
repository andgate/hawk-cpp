#ifndef HAWK_COMPILE_H
#define HAWK_COMPILE_H

#include "hkc/build.h"
#include "hkc/codegen.h"
#include "hkc/NodePrinter.h"
#include "hkc/parser/driver.h"

#include <string>
#include <memory>

namespace hkc
{
    class Compiler
    {
    private:
        Build_ptr build;
        
    public:
        Compiler(Build_ptr build)
        : build(build) {}
        
        void run();
        
        nmodule parse(const std::string& in_file);
        
        void gen_ir(const nmodule& module, const std::string& out_file);
        void gen_asm(const std::string& in_file, const std::string& out_file);
        void gen_obj(const std::string& in_file, const std::string& out_file);
        
        void link(const std::vector<std::string>& in_files);
    };
}

#endif
