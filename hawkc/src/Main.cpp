#include "Compiler.h"
#include "AST.h"
#include "Parser.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cerrno>

std::string get_file_contents(const std::string& filename)
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
void parseFile(const std::string& filename)
{
  std::string src_contents = get_file_contents(filename);
  auto ast = Parser::run(src_contents);

  Parser::debug(ast);
}

int main(int argc, char* argv[])
{
  bool debug = true;
  Compiler::run(debug);

  return 0;
}
