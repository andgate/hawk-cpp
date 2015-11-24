#pragma once

#include "Lexer/Token.h"

#include <string>

namespace Lexer
{

token_vector run(const std::string&);
void debug(const token_vector&);

}
