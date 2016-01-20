#pragma once

#include "hkc/build.h"

#include <string>
#include <memory>

namespace hkc
{
    class Compiler
    {
    private:
        Build_ptr build;
        
    public:
        Compiler(Build_ptr build) : build(build) {}
        
        void run();
    };
}
