#pragma once

#include "hkc/parser/ast.h"
#include "hkc/parser/ast/module.h"

#include <memory>

namespace hkc
{
	Ast_sptr parse(ast::Module_Vector_sptr modules);
}
