#pragma once

#include "AST.h"

#include <memory>

class Parser
{
public:
	Parser(shared_module_vector modules);

	void operator()();
	void debug();

private:
	shared_module_vector m_modules;

	void parse_top_level();

	void parse_top_level_exports();
	void parse_top_level_imports();
	
	void parse_top_level_config();
	void parse_top_level_typesig();
	void parse_top_level_function();

};
