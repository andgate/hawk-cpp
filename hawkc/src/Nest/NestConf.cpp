#include "Nest/NestConf.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <boost/filesystem.hpp>

boost::filesystem::path parse_path(const std::string& some_path_str, const std::string& base_path_str);
boost::filesystem::path parse_base_path(const std::string& base_path_str);

NestConf::NestConf()
	: m_name()
	, m_project_dir()
	, m_dirs()
	, m_files()
{}


// Throw exception is something is missing
NestConf::NestConf(Json::Value& value, const std::string& project_dir)
{
	m_name = value["name"].asString();

	m_project_dir = parse_base_path(project_dir).string();

	auto has_dir_path_input(!project_dir.empty());
	if(has_dir_path_input)
	{
		m_project_dir = parse_base_path(project_dir).string();
	}
	

	for (auto src_file : value["files"])
	{
		add_src_file(src_file.asString());
	}

	for (auto src_dir : value["dirs"])
	{
		add_src_dir(src_dir.asString());
	}
}


std::string NestConf::get_name()
{
	return m_name;
}

std::string NestConf::get_project_dir()
{
	return m_project_dir;
}

void NestConf::add_src_dir(const std::string& dir_path_str)
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

	m_dirs.insert(src_dir_path.string());
}


std::vector<std::string> NestConf::get_src_dirs()
{
	return std::vector<std::string>(m_dirs.begin(), m_dirs.end());
}

const std::string HAWK_FILE_EXTENSION = ".hk";
void NestConf::add_src_file(const std::string& file_path_str)
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
}

std::vector<std::string> NestConf::get_src_files()
{
	return std::vector<std::string>(m_files.begin(), m_files.end());
}

static const std::string CONF_FILE_TYPE = ".nest";

std::string NestConf::find(const std::string& nest_path_str)
{
	std::string conf_filename("");

	try
	{
		auto nest_path = parse_base_path(nest_path_str);

		if ( ! boost::filesystem::exists(nest_path) )
		{
			// throw exception
		}

		if ( boost::filesystem::is_regular_file(nest_path) )
		{
			// should be nest path, not regular file. Maybe adjust this?
			conf_filename = nest_path.string();
		}
		else if ( boost::filesystem::is_directory(nest_path) )
		{
			// Scan search directory for .nest file...
			for (boost::filesystem::directory_entry& x_directory_entry
				: boost::filesystem::directory_iterator(nest_path))
			{
				auto entry_path = x_directory_entry.path();

				auto is_conf_file =
					boost::filesystem::exists(entry_path)
					&& boost::filesystem::is_regular_file(entry_path)
					&& entry_path.has_extension()
					&& (entry_path.extension() == CONF_FILE_TYPE);

				if (is_conf_file)
				{
					conf_filename = entry_path.string();
				}
			}
		}
	}
	catch (const boost::filesystem::filesystem_error& ex)
	{
		std::cout << ex.what() << std::endl;
	}

	return conf_filename;
}

std::shared_ptr<NestConf> NestConf::load(const std::string& nest_path_str)
{
	std::string conf_path = find(nest_path_str);

	Json::Value root;
	Json::Reader reader;
	std::ifstream conf_istream(conf_path, std::ifstream::binary);

	bool parsingSuccessful = reader.parse(conf_istream, root, false);
	if (!parsingSuccessful)
	{
		std::cout << reader.getFormatedErrorMessages() << std::endl;

		// throw parse failed exception
		auto ex_msg("File format for \"" + conf_path + "\" is not valid \"" + CONF_FILE_TYPE + "\" format.");
		throw std::invalid_argument(ex_msg);
	}


	// Load succeeded.
	auto conf = std::make_shared<NestConf>(root, nest_path_str);
	return conf;
}





boost::filesystem::path parse_path(const std::string& some_path_str, const std::string& base_path_str)
{
	auto base_path = parse_base_path(base_path_str);
	auto some_path = boost::filesystem::path(some_path_str);
	
	// Path correction
	if (some_path.is_relative())
	{
		try
		{
			some_path = boost::filesystem::canonical(some_path, base_path);
		}
		catch (const boost::filesystem::filesystem_error& ex)
		{
			std::cout << ex.what() << std::endl;
		}
	}
	else
	{
		try
		{
			some_path = boost::filesystem::canonical(some_path);
		}
		catch (const boost::filesystem::filesystem_error& ex)
		{
			std::cout << ex.what() << std::endl;
		}
	}

	return some_path;
}

boost::filesystem::path parse_base_path(const std::string& base_path_str)
{
	auto base_path = boost::filesystem::path(base_path_str);

	// Base path correction
	if (base_path.is_relative())
	{
		try
		{
			base_path = boost::filesystem::canonical(base_path);
		}
		catch (const boost::filesystem::filesystem_error& ex)
		{
			std::cout << ex.what() << std::endl;
		}
	}

	return base_path;
}