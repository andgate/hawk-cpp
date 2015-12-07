#pragma once

#include "hkc/lex/token.h"

#include <string>
#include <memory>

namespace hkc
{

	namespace lex
	{
		void debug(const Token_Vector&);

		std::shared_ptr<lex::Token_Vector> lex(std::istream&);
		std::shared_ptr<lex::Token_Vector> lex();
		lex::Token_Vector_Shared lex(const std::string&);
	}
}
