#include "Parser.h"

#include "Lexer.h"

#include <iostream>

static ExprAST parse(const token_vector& tokens);

ExprAST Parser::run(const std::string& src)
{
  auto tokens = Lexer::run(src);
  Lexer::debug(tokens);

  return parse(tokens);
}

void Parser::debug(const ExprAST& ast)
{
  std::cout << std::endl;
  std::cout << "Parser Produced the following output" << std::endl;
}

static ExprAST parseNumericExpression()
{
  return NumberExprAST(0);
}

static ExprAST parse(const token_vector& tokens)
{
  return parseNumericExpression();
}
