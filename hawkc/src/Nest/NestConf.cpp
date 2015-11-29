#include "Nest/NestConf.h"


#include "tinydir.h"
#include "Util.h"

NestConf::NestConf()
	: m_name()
	, m_dirs()
	, m_files()
{}

NestConf::NestConf(Json::Value& root)
	: m_name(root["nest"].asString())
	, m_dirs(root["dirs"].size())
	, m_files(root["files"].size())
{
	for(auto src_dir : root["dirs"])
	{
		m_dirs.push_back(src_dir.asString());
	}

	for (auto src_file : root["files"])
	{
		m_files.push_back(src_file.asString());
	}
}

static const std::string CONF_FILE_TYPE = ".nest";

std::string NestConf::find(const std::string& search_dir)
{
	std::string conf_filename = "";

	tinydir_dir dir;
	tinydir_open(&dir, search_dir.c_str());

	while(dir.has_next)
	{
		tinydir_file file;
		tinydir_readfile(&dir, &file);
		std::string file_name(file.name);

		if (file_name.size() < (CONF_FILE_TYPE.size() + 1))
		{
			conf_filename = "";
		}

		int file_type_pos = file_name.size() - CONF_FILE_TYPE.size() - 1;
		std::string file_type(file_name.substr(file_type_pos, CONF_FILE_TYPE.size()));

		if (file_type == CONF_FILE_TYPE)
		{
			if (conf_filename == "")
				conf_filename = file_name;
		}

		tinydir_next(&dir);
	}

	tinydir_close(&dir);

	return conf_filename;
}

std::shared_ptr<NestConf> NestConf::load(const std::string& conf_path)
{
	// load json files
	std::string conf_src = Util::get_file_contents(conf_path);

	if(conf_src.length() > 0)
	{
		Json::Value root;
		Json::Reader reader;

		if (reader.parse(conf_src, root))
		{
			// Load succeeded.
			return std::make_shared<NestConf>(root);
		}
		// Load failed, could not parse.
	}

	// Load failed, file empty
	return std::make_shared<NestConf>();
}

std::shared_ptr<NestConf> NestConf::loadFromDir(const std::string& curr_dir)
{
	std::string conf_path = find(curr_dir);

	return load(conf_path);
}