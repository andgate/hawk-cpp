#include "Parser.h"

#include "Lexer.h"

#include <iostream>
#include <memory>

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

/*static ExprAST parseNumericExpression(const token_vector& tokens)
{
  return NumberExprAST(0);
}*/


/*static ExprAST parseProtoExpression(const token_vector& tokens)
{
  for(int i = 0; i < tokens.size(); i++)
  {
    auto token { tokens[i] };

    if(token == "=")
    {

    }
  }

  //const std::string &name, std::vector<std::string> Args

  return PrototypeAST(tokens);
}*/

static token_vector parseTypeSigStr(const int& token_index, const token_vector& tokens)
{
  int scope = tokens[token_index - 1]->getScope();
  int lineNum = tokens[token_index - 1]->getLineNum();

  token_vector out_tokens;

  for(int token_index_offset = 1; (token_index_offset+token_index) < tokens.size(); token_index_offset++)
  {
    auto curr_token = tokens[token_index + token_index_offset];

    if(curr_token->getId() != "->")
    {
      if(curr_token->getLineNum() == lineNum)
      {
        out_tokens.push_back(curr_token);
      } else if(curr_token->getScope() > scope)
      {
        out_tokens.push_back(curr_token);
      }
      else
      {
        return out_tokens;
      }
    }
  }
}

static TypeSigAST parseTypeSigAST(const int& token_index, const token_vector& tokens)
{
  auto name = tokens[token_index - 1];
  auto args = parseTypeSigStr(token_index, tokens);

  return TypeSigAST(name, args);
}

static ModuleAST parse(const token_vector& tokens)
{
  for(int token_index = 0; token_index < tokens.size(); token_index++)
  {
    auto token = tokens[token_index];

    if(token->getId() == ":")
    {
        parseTypeSigAST(token_index, tokens);
    }
  }

  return parseTypeSigAST(tokens);
}
