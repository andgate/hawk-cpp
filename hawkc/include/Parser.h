#pragma once

#include "AST.h"

namespace Parser
{

Expr run(const std::string& src);
void debug(const Expr& ast);

}
