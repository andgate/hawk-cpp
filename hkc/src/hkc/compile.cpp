#include "hkc/compile.h"

#include "hkc/parse.h"
#include "hkc/parser/Lexer.h"

void hkc::compile(Build_ptr build)
{
	// ----------------
	// Compiler Process
	// ----------------

  // 1. Lex tokens
  auto lexer = hkc::lex_files(build->src_files);

  // 2. Parse the token sets into an abstract syntax tree.
	//auto build_ast = parse(modules);

  // 3. Typecheck, optimize, extract entry points

	// 4. Pass entry points to code generator
  //    LLVM backend will generate the executables and libraries.
	//auto codes = hkc::code_gen(ast);

	// 4. Finally, write the binaries out to disk.
	//write_files(codes)????
}
