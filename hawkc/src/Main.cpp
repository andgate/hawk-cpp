#include <boost/spirit/include/qi.hpp>
#include <Core.h>
#include "Lexer.h"
#include "Parser.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cerrno>
using namespace std;

std::string get_file_contents(const string& filename)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    return(contents);
  }
  throw(errno);
}

// someday, this will return a list of strings or something
void parseFile(const string& filename)
{
  string src_contents = get_file_contents(filename);
  Lexer lex(src_contents);

  lex.debug();

  while(lex.hasToken())
  {
    auto tok = lex.getToken();
  }
}

int main(int argc, char* argv[])
{
    parseFile("bare.hk");

    return 0;
}
