#include <hkc/compile.h>
#include <hkc/build.h>

#include <boost/program_options.hpp>
#include <string>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char* argv[])
{   
    try
    {
        po::options_description generic("Generic options");
        generic.add_options()
            ("verbose,v", "Print extra information")
            ("version,V", "print version string")
            ("help,h", "produce help message")
            ;
            
        po::options_description config("Configuration");
        config.add_options()
            ("executable,e", po::value<std::string>(), "build executable")
            ("main-is", po::value<std::string>(), "define main function")
            ("library,l", po::value<std::string>(), "build library")
            ("debug,d", "build with debug information")
            ;
                        
        po::options_description hidden("Hidden options");
        hidden.add_options()
            ("input-file", po::value<std::vector<std::string>>(), "input file")
            ;

        po::options_description cmdline_options;
        cmdline_options.add(generic).add(config).add(hidden);
        
        po::options_description visible("Allowed options");
        visible.add(generic).add(config);
        
        po::positional_options_description p;
        p.add("input-file", -1);
        
        po::variables_map vm;
        //po::store(po::parse_command_line(argc, argv, cmdline_options), vm);
        po::store(po::command_line_parser(argc, argv).
                  options(cmdline_options).positional(p).run(), vm);
        po::notify(vm);

        /* Check for generic options */
        if (vm.count("help") || argc == 1)
        {
            std::cout << "Hawk Compiler, version 0.1" << endl;
            std::cout << visible << "\n";
            return 0;
        }
        
        if(vm.count("version"))
        {
            std::cout << "Hawk Compiler, version 0.1" << endl;
            return 0;
        }
        
        /* Sanity check */
        if(vm.count("executable") && vm.count("library"))
        {
            std::cout << "Cannot build an executable and a library at the same time." << endl;
            return 0;
        }
        
        /* Begin constructing a build config */
        auto build = std::make_shared<hkc::Build>();
        if (vm.count("executable"))
        {
            build->output = hkc::Build::BinOut;
            build->out_file = vm["executable"].as<std::string>();
        }
        else if (vm.count("library"))
        {
            build->output = hkc::Build::LibOut;    
            build->out_file = vm["library"].as<std::string>();
        }
        else
        {
            std::cout << "Please specify an output executable or library." << endl;
            return 0;
        }
        
        if(vm.count("input-file"))
        {
            build->in_files = vm["input-file"].as<std::vector<std::string>>();
        }
        else
        {
            std::cout << "Please specify at least one input file." << endl;
            return 0;
        }
        
        if(vm.count("debug"))
        {
            build->debug = true;
        }
        
        
        hkc::Compiler hkc(build);
        hkc.run();
    }
    catch(exception& e)
    {
        std::cout << e.what() << endl;
        return 1;
    }
    
    
    return 0;
}
