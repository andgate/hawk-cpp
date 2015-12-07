#pragma once
\
#include "hkc/symbol.h"
#include "hkc/lex/token.h"

#include <memory>
#include <string>
#include <vector>
#include <map>

#include <boost/any.hpp>

namespace hkc
{
	namespace ast
	{
		struct Module
		{
			std::string m_file_path;
			lex::Token_Vector_Shared m_tokens;
			std::vector<boost::any> m_asts;

			std::vector<Module> m_imports;
			std::vector<std::string> m_exports;

			std::map<std::string, std::vector<Symbol>> m_sym_table;
		};

		typedef std::vector<std::unique_ptr<Module>> Module_Vector;
		typedef std::shared_ptr<Module_Vector> Module_Vector_Shared;
	}
}