#include "Parser.h"

#include "Lexer.h"/l

#include <iostream>
#include <memory>

static Expr parse(const token_vector& tokens);

Expr Parser::run(const std::string& src)
{
  auto tokens = Lexer::run(src);
  Lexer::debug(tokens);

  return parse(tokens);
}

void Parser::debug(const Expr& ast)
{
  std::cout << std::endl;
  std::cout << "Parser Produced the following output" << std::endl;
}

/*static Expr parseNumericExpression(const token_vector& tokens)
{
  return Exprb(0);
}*/


/*static Expr parseProtoExpression(const token_vector& tokens)
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

/*static token_vector parseTypeSigStr(const int& token_index, const token_vector& tokens)
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

static std::shared_ptr<TypeSigAST> parseTypeSigAST(const int& token_index, const token_vector& tokens)
{
  auto name = tokens[token_index - 1];
  auto args = parseTypeSigStr(token_index, tokens);

  return std::make_shared<TypeSigAST>(TypeSigAST(name, args));
}*/

static std::unique_ptr<ModuleAST> parseModule(std::unique_ptr<ModuleAST> module_ptr)
{
	// Check the module expr to see is unparsed.
	// If so, then need to scan tokens for module name,
	// and module name definition conflicts.
	// Once module name is adjusted correctly, parsing begins.


	return module_ptr;
}

static Expr parse(const token_vector& tokens)
{
  return Expr();
}
