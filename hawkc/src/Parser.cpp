#include "Parser.h"

Parser::Parser(const std::string& src)
  : m_lex(src)
{}

Parser::Parser(const Parser& rhs)
  : m_lex(rhs.m_lex)
{}

Parser::Parser(Parser&& rhs)
  : m_lex(std::move(rhs.m_lex))
{}

Parser& Parser::operator=(const Parser& rhs)
{
  m_lex = rhs.m_lex;

  return *this;
}

Parser& Parser::operator=(Parser&& rhs)
{
  m_lex = std::move(rhs.m_lex);

  return *this;
}


ExprAST Parser::parseNumericExpression()
{
  return NumberExprAST(0);
}

#include <iostream>

void Parser::debug()
{
  m_lex.debug();
  std::cout << "should print some sort of ast";
}
