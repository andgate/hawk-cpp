#include "Lexer.h"

#include <iostream>


static token_vector tokenize(const std::string& src);
static void eraseComments(token_vector& tokens);


token_vector Lexer::run(const std::string& src)
{
  auto tokens = tokenize(src);
  eraseComments(tokens);
  return tokens;
}

void Lexer::debug(const token_vector& tokens)
{
  std::cout << "Lexer produced the following tokens at the given line number - scope..." << std::endl;
  for(auto token : tokens)
  {
    std::cout << token->getLineNum() << " - " << token->getScope() << ": " << token->getId() << std::endl;
  }
}


// This whole thing is NASTY! Can I put this all into a throw-away class?
static token_vector tokenize(const std::string& src)
{
  token_vector tokens;

  std::string tok_acc;
  char delim = ' ';

  int line_num = 1;

  int ws_count = 0;
  bool mustCountScope = true;
  bool isOnToken = false;

  int start_token = 0;
  int end_token = 0;

  for(int i = 0; i < src.size(); i++)
  {
    char curr_tok = src[i];

    if(curr_tok == '\n')
    {
      if(isOnToken)
      {
        // Ooops! Should make new token in factory!
        tokens.push_back(std::make_shared<Token>(tok_acc, line_num, ws_count));
        tok_acc.clear();
        isOnToken = false;
      }

      ++line_num;
      ws_count = 0;
      mustCountScope = true;
    }
    else if(curr_tok != delim)
    {
      // create token, add to tokens
      tok_acc += curr_tok;
      mustCountScope = false;
      isOnToken = true;
    }
    else if(curr_tok == delim)
    {
      if(isOnToken)
      {
        tokens.push_back(std::make_shared<Token>(tok_acc, line_num, ws_count));
        tok_acc.clear();
        isOnToken = false;
      }

      if(mustCountScope)
      {
        ++ws_count;
      }
    }
  }

  return tokens;
}

static void eraseSingleLineComment(token_vector& tokens, const int& start_index)
{
  int comment_line_num = tokens[start_index]->getLineNum();
  auto start_comment = tokens.begin() + start_index;
  int token_index = start_index;

  while( (token_index < tokens.size())
      && (comment_line_num == tokens[token_index]->getLineNum()))
  { ++token_index; }

  auto end_comment = tokens.begin() + token_index;

  tokens.erase(start_comment, end_comment);
}

static void eraseMultiLineComment(token_vector& tokens, const int& start_index)
{
  auto start_comment = tokens.begin() + start_index;
  int token_index = start_index;

  while( (token_index < tokens.size())
      && (tokens[token_index]->getId() != "*/"))
  { ++token_index; }

  auto end_comment = tokens.begin() + token_index + 1;

  tokens.erase(start_comment, end_comment);
}

static void eraseComments(token_vector& tokens)
{
  for(int token_index = 0; token_index < tokens.size(); token_index++)
  {
    auto token_id = tokens[token_index]->getId();

    if(token_id == "//")
    {
      eraseSingleLineComment(tokens, token_index);
      --token_index;
    }
    else if (token_id == "/*")
    {
      eraseMultiLineComment(tokens, token_index);
      --token_index;
    }
  }
}
