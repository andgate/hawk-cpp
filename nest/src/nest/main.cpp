#include "nest/nest.h"
#include "hkc/compile.h"
#include "hkc/build.h"

#include <boost/program_options.hpp>
#include <string>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
	po::options_description desc("Allowed options");
	desc.add_options()
		("build", "build the project")
		("help", "produce help message")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (argc == 1 && vm.count("build"))
	{
		std::cout << "Building project..." << "\n";

        // 1. Find config file
        // 2. Load config with path
        // 3. generate build command with config
        // 4. Call out to hkc compiler with the build command
        hkc::compile(
            Nest::gen_build(		// 3
                Nest::load(			// 2
                    Nest::find()	// 1
                    )
                )
            );

		return 0;
	}

	if( vm.count("help") )
	{
		std::cout << desc << "\n";
		return 0;
	}
}
