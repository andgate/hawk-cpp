#include "Nest.h"

#include "Nest/NestConf.h"
#include "Lexer.h"
#include "Parser.h"

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>

Nest::Nest()
  : m_name()
  , m_modules(std::make_shared<module_vector>())
{}

Nest::Nest(const Nest& rhs)
	: m_name()
	, m_modules(rhs.m_modules)
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

		auto module_tokens = Lexer::run(src_txt_in);
		Lexer::debug(*module_tokens);

		auto module = std::make_shared<ModuleAST>(module_file_path, module_tokens);
		nest->m_modules->push_back(module);
	}


	return nest;
}

void Nest::build()
{
	Parser parser(m_modules);
	parser(); // run parser

	// ask parser for program asts
	// send program asts to code gen
	// produce bytecode, optimize bytecode
	// then compile bytecode and write out
	// the executable.
}


void Nest::debug()
{
	//std::cout << "*print debug info*" << std::endl;
}