#pragma once

#include "AST/Expr.h"

#include "Lexer/Token.h"

#include <memory>
#include <vector>

class UnparsedExpr : public Expr
{
public:
	UnparsedExpr(std::shared_ptr<token_vector> Val)
		: Val(Val)
	{}

private:
	std::shared_ptr<token_vector> Val;
};