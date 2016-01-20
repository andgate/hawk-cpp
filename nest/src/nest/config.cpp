#include "nest/config.h"

#include <boost/filesystem.hpp>
#include <hkc/utils/ReadFile.h>
#include <iostream>
#include <yaml-cpp/yaml.h>

namespace fs = boost::filesystem;

Nest::Config_ptr Nest::load_conf()
{
    auto config = std::make_shared<Nest::Config>();
    
    fs::path curr_path( fs::current_path() );
    config->project_dir = curr_path.string();
    
    auto src = load_file("nest.yaml"); 
    YAML::Node node = YAML::Load(src.c_str());
    config->name = node["project"].as<std::string>();
    
    /*if(node["dirs"].IsSequence())
     *   {
     *          // grab the list
}
else
{
config->dirs.push_back(node["dirs"].as<std::string>());
}*/
    
    config->files.push_back(node["files"].as<std::string>());
    
    /*for (auto src_dir : node["dirs"])
     *   {
     *          config->dirs.push_back(src_dir.asString());
}

for (auto src_file : node["files"])
{
config->files.push_back(src_file.asString());
}*/
    
    return config;
}