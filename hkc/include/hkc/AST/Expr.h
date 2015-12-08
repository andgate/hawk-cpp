#pragma once

#include "hkc/ast.h"

/// ExprAST - Base class for all expression nodes.
namespace hkc
{
	namespace ast
	{
		struct Expr
		{
			// some stats about expressions
			Ast_sptr root;
		};
	}
}