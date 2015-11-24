#pragma once

#include <string>
#include <vector>

namespace whitespace
{
int lcount(const std::string& line);
std::vector<int> lcounts(const std::string& contents);
}
