#include "Nest.h"

#include "Nest/NestConf.h"
#include "Util.h"
#include "Lexer.h"

Nest::Nest()
  : m_modules()
{}

Nest::Nest(const Nest& rhs)
	: m_modules(rhs.m_modules)
{}

Nest::Nest(Nest&& rhs)
	: m_modules(std::move(rhs.m_modules))
{}

Nest& Nest::operator=(const Nest& rhs)
{
	m_modules = rhs.m_modules;
	return *this;
}

Nest& Nest::operator=(Nest&& rhs)
{
	m_modules = std::move(rhs.m_modules);
	return *this;
}


std::shared_ptr<Nest> Nest::load(const std::string& nest_dir)
{
	auto nest = std::make_shared<Nest>();

	auto conf = NestConf::loadFromDir(nest_dir);
	for(auto module_name : conf->getFiles())
	{
		auto src_txt = Util::get_file_contents(module_name);

		auto tokens = Lexer::run(src_txt);
		Lexer::debug(tokens);

		auto module_body = std::make_shared<UnparsedExpr>(tokens);

		auto module = std::make_shared<ModuleAST>(module_name, module_body);
		nest->m_modules.push_back(module);
	}


	return nest;
}

void Nest::build()
{
}


#include <iostream>

void Nest::debug()
{
	std::cout << "*print debug info*" << std::endl;
}