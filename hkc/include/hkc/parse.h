#pragma once

#include "hkc/ast.h"
#include "hkc/ast/module.h"

#include <memory>

namespace hkc
{
	Ast_ptr parse(ast::Module_Vector_Shared modules);
}
