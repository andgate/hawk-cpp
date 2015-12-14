#include "hkc/compile.h"

#include "hkc/parse.h"
#include "hkc/lex.h"
#include "hkc/ast/module.h"

void hkc::compile(Build_ptr build)
{
	// ----------------
	// Compiler Process
	// ----------------

	// 1. Loop through and lex each file in the build.
    auto modules = hkc::lex::lex(build->src_files);

	// 2. Parse the resulting module list.
	auto build_ast = parse(modules);

	// 3. Codegen the resulting ast.
	//auto codes = hkc::code_gen(ast);

	// 4. Finally, write the binary and library files.
	//write_files(codes)????
}
