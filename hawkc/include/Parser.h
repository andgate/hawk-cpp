#pragma once

#include "AST.h"

#include "Lexer.h"
#include "Lexer/Token.h"

#include <memory>

class Parser
{
public:
  Parser(const std::string&);
  Parser(const Parser&);
  Parser(Parser&&);

  Parser& operator=(const Parser&);
  Parser& operator=(Parser&&);

  void parse();

  void debug();

private:
  Lexer m_lex;

  ExprAST parseNumericExpression();
};
