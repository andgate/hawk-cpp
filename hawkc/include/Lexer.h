#pragma once

#include "Lexer/Token.h"

#include <memory>
#include <string>
#include <vector>

class Lexer
{
public:
  Lexer(const std::string&);
  Lexer(const Lexer&);
  Lexer(Lexer&&);

  Lexer& operator=(const Lexer&);
  Lexer& operator=(Lexer&&);

  bool hasToken();
  std::shared_ptr<Token> getToken();

  void debug();

private:
  std::vector<std::shared_ptr<Token>> m_tokens;
  int m_currTok;

  void tokenize(const std::string&);
};
