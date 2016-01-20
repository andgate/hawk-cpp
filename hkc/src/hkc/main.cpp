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

  // Eventually, parse compiler commands into compiler config (Build.h)

  if (argc == 1)
  {
    std::cout << "Compiling..." << "\n";

    auto build = std::make_shared<hkc::Build>();
    build->src_files.push_back("main.hk");

    hkc::Compiler hkc(build);
    hkc.run();

    return 0;
  }

  if (vm.count("help"))
  {
    std::cout << desc << "\n";
    return 0;
  }

// compile the given build and return
}
