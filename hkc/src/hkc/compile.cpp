#include "hkc/compile.h"

#include "hkc/parse.h"
#include "hkc/lex.h"
#include "hkc/ast/module.h"

#include <fstream>

void hkc::compile(std::unique_ptr<Build>& build)
{
	// Compiler Process
	// 1. Loop through and lex each file
	//    in the build.
	// 2. Parse the resulting module list.
	// 3. Codegen the resulting ast.
	// 4. Finally, write the binary and library files.

	ast::Module_Vector modules;

	for(auto src_file : build->src_files)
	{
		std::ifstream src_file_in(src_file, std::ifstream::binary);
		auto tokens = hkc::lex::lex(src_file_in);
		// Construct empty modules
		// Insert empty modules into a list
		modules.push_back(std::make_unique<ast::Module>());
	}
}
