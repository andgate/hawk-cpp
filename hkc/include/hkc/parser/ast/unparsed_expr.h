#pragma once

#include "hkc/parser/ast/expr.h"
#include "hkc/lex/token.h"

#include <memory>
#include <vector>

namespace hkc
{
	namespace ast
	{
		struct UnparsedExpr : Expr
		{
			std::shared_ptr<lex::Token_Vector> Val;
		};
	}
}
