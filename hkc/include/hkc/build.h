#pragma once

#include <string>
#include <vector>
#include <memory>

namespace hkc
{

	struct Build
	{
		std::vector<std::string> src_files;
	};

	typedef std::shared_ptr<Build> Build_ptr;

}
