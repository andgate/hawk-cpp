#pragma once

#include <memory>
#include <string>
#include <vector>

namespace Nest
{

struct Config
{
    // System data
    std::string project_dir;

    // Project data
    std::string name;
    std::vector<std::string> dirs;
    std::vector<std::string> files;
};

typedef std::vector<Config> Configs;
typedef std::shared_ptr<Config> Config_ptr;

Config_ptr load_conf();

}
