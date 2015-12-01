#include "AST.h"
#include "Compiler.h"
#include "Parser.h"
#include "Util.h"

#include <string>


// Kept around to show how parser is used.
// Delete when building nests does this.
void parseFile(const std::string& filename)
{
  std::string src_contents = Util::get_file_contents(filename);
  auto ast = Parser::run(src_contents);

  Parser::debug(ast);
}

int main(int argc, char* argv[])
{
	auto debug = true;

	if (argc <= 1)
	{
		Compiler::run(debug);
	}
	else
	{
		std::string run_path(argv[1]);
		Compiler::run(run_path, debug);
	}

	return 0;
}
