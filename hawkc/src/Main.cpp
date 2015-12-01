#include "Compiler.h"

#include <string>


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
