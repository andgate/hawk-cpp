#pragma once

#include <iostream>

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

namespace nest
{

	static std::string resolve_path(
		const std::string& p_str,
		const std::string& base_str = boost::filesystem::current_path().string()
		)
	{
		boost::filesystem::path p(p_str);
		boost::filesystem::path base(base_str);

		//boost::filesystem::path result(canonical(p, base));
		boost::filesystem::path result;

		try
		{
			if (base.is_relative())
			{
				base = resolve_path(base_str);
			}

			if (p.is_relative())
			{
				p = fs::canonical(p, base);
			}

			if (fs::is_symlink(p))
			{
				p = fs::read_symlink(p);
			}
		}
		catch (const boost::filesystem::filesystem_error& ex)
		{
			// location probably doesn't exist
			// can't make canonical
			std::cout << ex.what() << std::endl;
		}


		return result.string();
	}

}