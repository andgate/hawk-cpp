#pragma once

#include "hkc/parser/Token.h"

#include <string>
#include <vector>

namespace hkc
{

std::vector<Tokens> lex_files(std::vector<std::string> src_files);

class Lexer
{
public:
  Lexer() = delete;
  Lexer(std::string src, std::string src_path);
  ~Lexer();

  Tokens get_toks();

  enum State
  {
    top_level = -1,
  };

private:
  std::string m_src;
  std::string m_src_path;
  int m_pos;
  int m_line_num;
  int m_col_num;
  std::vector<State> states;
};

};
