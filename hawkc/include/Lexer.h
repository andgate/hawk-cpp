#pragma once

#include "Lexer/Token.h"

#include <string>
#include <fstream>

namespace Lexer
{

std::shared_ptr<token_vector> run(const std::string&);
std::shared_ptr<token_vector> run(std::istream&);
std::shared_ptr<token_vector> run();

void debug(const token_vector&);

}
