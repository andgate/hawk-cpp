#pragma once

#include "AST/Expr.h"

#include "Lexer/Token.h"


#include <memory>
#include <vector>

class UnparsedExpr : public Expr
{
public:
	UnparsedExpr(token_vector Val)
		: Val(std::move(Val))
	{}

private:
	token_vector Val;
};