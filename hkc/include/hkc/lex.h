#pragma once

#include "ast/module.h"

#include <string>
#include <memory>
#include <vector>

namespace hkc
{

	namespace lex
	{
		hkc::ast::Module_Vector_sptr lex(std::vector<std::string> src_files);
		//void debug(const Token_Vector&);
	}
}
