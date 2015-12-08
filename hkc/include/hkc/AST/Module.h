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
			std::string file_path = "";
			lex::Token_Vector_sptr tokens = std::make_shared<hkc::lex::Token_Vector>();
			std::vector<boost::any> asts = std::vector<boost::any>();

			std::vector<Module> imports;
			std::vector<std::string> exports;

			std::map<std::string, std::vector<Symbol>> symbol_table;
		};

		typedef std::unique_ptr<Module> Module_ptr;
		typedef std::shared_ptr<Module> Module_sptr;

		typedef std::vector<Module_sptr> Module_Vector;
		typedef std::shared_ptr<Module_Vector> Module_Vector_sptr;
	}
}