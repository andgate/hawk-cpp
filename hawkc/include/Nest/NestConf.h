#pragma once

#include <string>
#include <memory>
#include <vector>

class NestConf
{
public:
  std::vector<std::string> m_src_dirs;
  std::vector<std::string> m_src_files;
};
