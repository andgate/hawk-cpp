#include "nest/Config.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <boost/filesystem.hpp>


// Throw exception is something is missing
/*Config::Config(Json::Value& value, const std::string& project_dir, const std::string& project_file)
	: m_project_dir(project_dir)
	, m_project_file(project_file)
	, m_name(value["name"].asString())
	, m_dirs()
	, m_files()
{
	// ensure valid json?
	//m_name = value["name"].asString();

	for (auto src_dir : value["dirs"])
	{
		m_dirs.insert(src_dir.asString());
	}

	for (auto src_file : value["files"])
	{
		m_files.insert(src_file.asString());
	}
}

void add_src_dir(const std::string& dir_path_str)
{
	auto src_dir_path = parse_path(dir_path_str, m_project_dir);

	auto exists = boost::filesystem::exists(src_dir_path);
	if (!exists)
	{
		auto ex_msg("Unable to add path (does not exist): " + src_dir_path.string());
		throw std::invalid_argument(ex_msg);
	}

	auto is_directory = boost::filesystem::is_directory(src_dir_path);
	if (!is_directory)
	{
		auto ex_msg("Unable to add path (was not directory): " + src_dir_path.string());
		throw std::invalid_argument(ex_msg);
	}

	for (boost::filesystem::directory_entry& x_directory_entry
		: boost::filesystem::directory_iterator(src_dir_path))
	{
		auto entry_path = x_directory_entry.path();
		if(boost::filesystem::is_regular_file(entry_path))
		{
			add_src_file(entry_path.string());
		}
		else if(boost::filesystem::is_directory(entry_path))
		{
			add_src_dir(entry_path.string());
		}
		else
		{
			// Not a dir or valid file
			// Not added to project.
			// Silently ignored.
		}
	}

	m_dirs.push_back(src_dir_path.string());
}


std::vector<std::string> get_src_dirs()
{
	return std::vector<std::string>(m_dirs.begin(), m_dirs.end());
}

const std::string HAWK_FILE_EXTENSION = ".hk";
void add_src_file(const std::string& file_path_str)
{
	auto src_file_path = parse_path(file_path_str, m_project_dir);

	auto missing = !boost::filesystem::exists(src_file_path);
	if (missing)
	{
		auto ex_msg("Unable to add file (does not exist): " + src_file_path.string());
		throw std::invalid_argument(ex_msg);
	}

	auto not_regular_file = !boost::filesystem::is_regular_file(src_file_path);
	if (not_regular_file)
	{
		auto ex_msg("Unable to add file (is not regular file): " + src_file_path.string());
		throw std::invalid_argument(ex_msg);
	}

	auto has_no_extension = !src_file_path.has_extension();
	if (has_no_extension)
	{
		return;
	}

	auto not_hawk_source = src_file_path.extension() != HAWK_FILE_EXTENSION;
	if (not_hawk_source)
	{
		return;
	}

	// Add the validated path to the list
	m_files.insert(src_file_path.string());
}*/
