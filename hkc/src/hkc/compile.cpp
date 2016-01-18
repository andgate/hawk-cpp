#include "hkc/compile.h"

#include "hkc/codegen.h"
#include "hkc/NodePrinter.h"
#include "hkc/parser/driver.h"

#include <iostream>

class CodeGenContext;

void hkc::compile(Build_ptr build)
{
	// ----------------
	// Compiler Process
	// ----------------

    hawk_driver driver;
    driver.trace_parsing = true;
    driver.trace_scanning = false;
    
    for(std::string src_path : build->src_files)
    {
        if(!driver.parse(src_path))
        { 
            auto programBlock = driver.result;
            
            NodePrinter printer;
            
            programBlock->accept(printer);
            
            CodeGen gen;
            programBlock->accept(gen);
            
            CodeGenContext context;
            context.generateCode(*programBlock);
            context.runCode();
        }
    }
    
  // 1. Lex tokens

  // 2. Parse the token sets into an abstract syntax tree.
	//auto build_ast = parse(modules);

  // 3. Typecheck, optimize, extract entry points

	// 4. Pass entry points to code generator
  //    LLVM backend will generate the executables and libraries.
	//auto codes = hkc::code_gen(ast);

	// 4. Finally, write the binaries out to disk.
	//write_files(codes)????
}
