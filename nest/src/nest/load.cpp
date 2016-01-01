#include "nest/load.h"

#include "nest/constants.h"

#include <iostream>
#include <fstream>

#include <boost/filesystem.hpp>
#include <yaml-cpp/yaml.h>

namespace fs = boost::filesystem;

// Assumes given path is absolute and valid
// Input is expected to come from find()
Nest::Config_ptr Nest::load(const std::string& config_path_str)
{
	std::ifstream config_txt_in(config_path_str, std::ifstream::binary);
	std::string config_txt((std::istreambuf_iterator<char>(config_txt_in)), std::istreambuf_iterator<char>());

	auto config = std::make_shared<Nest::Config>();
	auto config_path = fs::path(config_path_str);
	auto project_root = config_path.parent_path();
	config->project_dir = project_root.string();
	config->project_file = config_path_str;

	YAML::Node node = YAML::Load(config_txt.c_str());
	config->name = node["project"].as<std::string>();

	/*if(node["dirs"].IsSequence())
	{
		// grab the list
	}
	else
	{
		config->dirs.push_back(node["dirs"].as<std::string>());
	}*/

	config->files.push_back(node["files"].as<std::string>());

	/*for (auto src_dir : root["dirs"])
	{
		config->dirs.push_back(src_dir.asString());
	}

	for (auto src_file : root["files"])
	{
		config->files.push_back(src_file.asString());
	}*/

	return config;
}
