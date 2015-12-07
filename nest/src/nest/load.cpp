#include "nest/Load.h"

#include "nest/constants.h"

#include <iostream>
#include <fstream>

#include <boost/filesystem.hpp>
#include <json/json.h>

namespace fs = boost::filesystem;

// Assumes given path is absolute and valid
// Input is expected to come from find()
Nest::Config_ptr Nest::load(const std::string& config_path_str)
{
	Json::Value root;
	Json::Reader reader;
	std::ifstream config_txt_in(config_path_str, std::ifstream::binary);

	bool parsingSuccessful = reader.parse(config_txt_in, root, false);
	if (!parsingSuccessful)
	{
		std::cout << reader.getFormatedErrorMessages() << std::endl;

		// throw parse failed exception
		auto ex_msg("File format for \"" + config_path_str + "\" is not valid \"" + NEST_FILE_EXTENSION + "\" format.");
		throw std::invalid_argument(ex_msg);
	}

	auto config = std::make_unique<Nest::Config>();
	auto config_path = fs::path(config_path_str);
	auto project_root = config_path.parent_path();

	config->project_dir = project_root.string();
	config->project_file = config_path_str;
	config->name = root["name"].asString();

	for (auto src_dir : root["dirs"])
	{
		config->dirs.push_back(src_dir.asString());
	}

	for (auto src_file : root["files"])
	{
		config->files.push_back(src_file.asString());
	}

	return config;
}