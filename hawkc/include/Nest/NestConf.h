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

	std::string getName();
	std::vector<std::string> getDirs();
	std::vector<std::string> getFiles();
	std::vector<std::string> getModules();

private:
	std::string				 m_name;
	std::vector<std::string> m_dirs;
	std::vector<std::string> m_files;

	static std::shared_ptr<NestConf> load(const std::string&);
	static std::string find(const std::string&);
};
