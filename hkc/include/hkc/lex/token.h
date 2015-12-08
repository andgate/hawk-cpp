#pragma once

#include <memory>
#include <string>
#include <vector>

namespace hkc
{
	namespace lex
	{
		struct Token
		{
			std::string tok;
			int tok_num;
			int line_num;
			int line_tok_num;
			int scope_num;
		};

		typedef std::shared_ptr<Token> Token_sptr;

		typedef std::vector<std::shared_ptr<Token>> Token_Vector;
		typedef std::shared_ptr<Token_Vector> Token_Vector_sptr;
	}
}