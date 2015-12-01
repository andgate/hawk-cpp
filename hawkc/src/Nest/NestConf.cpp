#include "Nest/NestConf.h"

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <boost/filesystem.hpp>

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

	m_project_dir = boost::filesystem::canonical
		(boost::filesystem::path(project_dir))
		.string();

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
	auto src_dir_path = boost::filesystem::canonical(boost::filesystem::path(dir_path_str)
		, boost::filesystem::path(m_project_dir));

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


std::vector<std::string> NestConf::get_src_dirs()
{
	return m_dirs;
}

const std::string HAWK_FILE_EXTENSION = ".hk";
void NestConf::add_src_file(const std::string& file_path_str)
{
	auto src_file_path = boost::filesystem::canonical(boost::filesystem::path(file_path_str)
      , boost::filesystem::path(m_project_dir));

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
	m_files.push_back(src_file_path.string());
}

std::vector<std::string> NestConf::get_src_files()
{
	return m_files;
}

static const std::string CONF_FILE_TYPE = ".nest";

static std::unique_ptr<boost::filesystem::path>
resolve_run_path(const std::string& run_dir_str)
{
	auto run_dir_path(std::make_unique<boost::filesystem::path>(run_dir_str));

	if (!boost::filesystem::is_directory(*run_dir_path))
	{
		run_dir_path = std::make_unique<boost::filesystem::path>(boost::filesystem::initial_path());
	}

	return run_dir_path;
}

std::string NestConf::find(const std::string& search_dir)
{
	std::string conf_filename("");

	try
	{
		auto search_path = boost::filesystem::canonical
			(boost::filesystem::path(search_dir));

		if ( ! boost::filesystem::exists(search_path) )
		{
			// throw exception
		}

		if ( boost::filesystem::is_regular_file(search_path) )
		{
			// should be search path, not regular file. Maybe adjust this?
		}

		if ( boost::filesystem::is_directory(search_path) )
		{
			// Scan search directory for .nest file...
			for (boost::filesystem::directory_entry& x_directory_entry
				: boost::filesystem::directory_iterator(search_path))
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

std::shared_ptr<NestConf> NestConf::load(const std::string& search_path)
{
	std::string conf_path = find(search_path);

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

	auto conf = std::make_shared<NestConf>(root);
	
	// Load succeeded.
	return conf;
}