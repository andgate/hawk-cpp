#ifndef _HKC_BUILD_H_
#define _HKC_BUILD_H_

#include <string>
#include <vector>
#include <memory>

namespace hkc
{

	struct Build
	{
        Build() : in_files() {}
        
        std::string build_dir = "build";
        std::string out_file;
        std::string main_is = "main";
		std::vector<std::string> in_files;
        
        enum Output { BinOut, LibOut };
        Output output = BinOut;
        
        bool debug = false;
	};

	typedef std::shared_ptr<Build> Build_ptr;

}

#endif