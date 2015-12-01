#pragma once

#include <string>
#include <memory>
#include <vector>
#include <set>

#include "json/json.h"


class NestConf
{
public:
	NestConf();
	NestConf(Json::Value& value, const std::string& project_dir = ".");

	static std::shared_ptr<NestConf> load(const std::string&);

	std::string get_name();
	
	void add_src_dir(const std::string&);
	std::vector<std::string> get_src_dirs();

	void add_src_file(const std::string&);
	std::vector<std::string> get_src_files();

	std::string get_project_dir();

	//std::vector<std::string> getModules();

private:
	std::string				 m_name;
	std::string				 m_project_dir;
	std::set<std::string> m_dirs; // todo: make std::set
	std::set<std::string> m_files; // todo: make std::set

	static std::string find(const std::string& nest_path_str);
};
