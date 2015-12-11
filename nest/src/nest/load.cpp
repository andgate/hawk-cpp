#include "nest/Load.h"

#include "nest/constants.h"

#include <iostream>
#include <fstream>

#include <boost/filesystem.hpp>
#include <json/json.h>
#include "SipYAML.hpp"

namespace fs = boost::filesystem;

// Assumes given path is absolute and valid
// Input is expected to come from find()
Nest::Config_ptr Nest::load(const std::string& config_path_str)
{
	std::ifstream config_txt_in(config_path_str, std::ifstream::binary);
	std::string config_txt((std::istreambuf_iterator<char>(config_txt_in)), std::istreambuf_iterator<char>());
	
	Sip::YAMLDocumentUTF8 src_yml;
	src_yml.parse(config_txt.c_str());

	auto config = std::make_unique<Nest::Config>();
	auto config_path = fs::path(config_path_str);
	auto project_root = config_path.parent_path();
	config->project_dir = project_root.string();
	config->project_file = config_path_str;

	auto node = src_yml.firstChild();
	while(node)
	{
		if (node->key() == "name")
		{
			config->name = node->value();
		}
		else if (node->key() == "dirs")
		{

		}
		else if(node->key() == "files")
		{
			
		}
		else
		{
			// Unrecognized node
		}

		node = node->nextSibling();
	}

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