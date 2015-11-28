#pragma once

#include <string>

namespace Compiler {

void run(const bool& debug = false);
void run(const std::string& run_dir = ".", const bool& debug = false);

};
