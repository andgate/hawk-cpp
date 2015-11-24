#pragma once

#include "AST.h"

namespace Parser
{

ExprAST run(const std::string& src);
void debug(const ExprAST& ast);

}
