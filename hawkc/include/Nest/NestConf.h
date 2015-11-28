#pragma once

#include <string>
#include <memory>
#include <vector>

#include "json/json.h"


class NestConf
{
public:
	NestConf();
	NestConf(Json::Value&value);

	static std::shared_ptr<NestConf> loadFromDir(const std::string&);

private:
  std::vector<std::string> m_src_dirs;
  std::vector<std::string> m_src_files;

  static std::shared_ptr<NestConf> load(const std::string&);
  static std::string find(const std::string&);
};
