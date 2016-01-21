#include "nest/nest.h"

#include <stdlib.h> 

#include <boost/program_options.hpp>
#include <string>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char* argv[])
{
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
	;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);
    
    if( vm.count("help"))
    {
        std::cout << desc << "\n";
        return 0;
    }

    std::cout << "Building project..." << std::endl;

    system("hkc -e test main.hk");

    return 0;
}
