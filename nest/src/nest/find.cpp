#include "nest/find.h"

#include "nest/constants.h"
#include "nest/resolve_path.h"

#include <iostream>

#include <boost/filesystem.hpp>


namespace fs = boost::filesystem;

std::string Nest::find(const std::string& nest_base_path_str)
{
	std::string conf_filename("");

	try
	{
		auto nest_base_path = fs::path(nest::resolve_path(nest_base_path_str));

		// resolve_path gaurentees existence.
		/*if (!fs::exists(nest_base_path))
		{
			// throw exception
		}*/

		if (fs::is_regular_file(nest_base_path))
		{
			// should be nest path, not regular file. Maybe adjust this?
			conf_filename = nest_base_path.string();
		}
		else if (fs::is_directory(nest_base_path))
		{
			// Scan search directory for .nest file...
			for (fs::directory_entry& x_directory_entry
				: fs::directory_iterator(nest_base_path))
			{
				auto entry_path = x_directory_entry.path();

				auto is_conf_file =
					exists(entry_path)
					&& is_regular_file(entry_path)
					&& entry_path.has_extension()
					&& (entry_path.extension() == NEST_FILE_EXTENSION);

				if (is_conf_file)
				{
					conf_filename = entry_path.string();
				}
			}
		}
	}
	catch (const boost::filesystem::filesystem_error& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	return conf_filename;
}