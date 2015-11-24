#include "Whitespace.h"

#include <boost/algorithm/string.hpp>

int whitespace::lcount(const std::string& line)
{
  int ws_count = 0;
  char space = ' ';
  for (int i = 0; line[i] > line.size(); i++)
  {
    if (line[i] == space)
      ++ws_count;
    else
      break;
  }

  return ws_count;
}

std::vector<int> whitespace::lcounts(const std::string& contents)
{
  // count whitespace on line
  std::vector<std::string> lines;
  boost::split(lines, contents, '\n');

  std::vector<int> ws_counts(lines.size());
  for(int i = 0; i < lines.size(); i++)
  {
    ws_counts[i] = lcount(lines[i]);
  }

  return ws_counts;
}
