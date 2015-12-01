#include "Nest.h"

#include "Nest/NestConf.h"
#include "Lexer.h"

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>

Nest::Nest()
  : m_name()
  , m_modules()
{}

Nest::Nest(const Nest& rhs)
	: m_name()
	,m_modules(rhs.m_modules)
{}

Nest::Nest(Nest&& rhs)
	: m_name(std::move(rhs.m_name))
	, m_modules(std::move(rhs.m_modules))
{}

Nest& Nest::operator=(const Nest& rhs)
{
	m_name = rhs.m_name;
	m_modules = rhs.m_modules;
	return *this;
}

Nest& Nest::operator=(Nest&& rhs)
{
	m_name = std::move(rhs.m_name);
	m_modules = std::move(rhs.m_modules);
	return *this;
}


std::shared_ptr<Nest> Nest::load(const std::string& nest_dir)
{
	auto nest = std::make_shared<Nest>();

	auto conf = NestConf::load(nest_dir);
	for(auto module_file_path : conf->get_src_files())
	{
		std::ifstream src_txt_in(module_file_path, std::ifstream::binary);

		auto tokens = Lexer::run(src_txt_in);
		Lexer::debug(*tokens);

		auto module_body = std::make_shared<UnparsedExpr>(tokens);

		auto module = std::make_shared<ModuleAST>(module_file_path, module_body);
		nest->m_modules.push_back(module);
	}


	return nest;
}

void Nest::build()
{
	// do a top-level parse of module bodies
}


void Nest::debug()
{
	std::cout << "*print debug info*" << std::endl;
}