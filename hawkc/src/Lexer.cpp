#include "Lexer.h"

#include <iostream> // cout

Lexer::Lexer(const std::string& src)
  : m_currTok(0)
{
  tokenize(src);
  eraseComments();
}

Lexer::Lexer(const Lexer& rhs)
  : m_tokens(rhs.m_tokens){}

Lexer::Lexer(Lexer&& rhs)
  : m_tokens(std::move(rhs.m_tokens))
{}

Lexer& Lexer::operator=(const Lexer& rhs)
{
  m_tokens = rhs.m_tokens;

  return *this;
}

Lexer& Lexer::operator=(Lexer&& rhs)
{
  m_tokens = std::move(rhs.m_tokens);

  return *this;
}

bool Lexer::hasToken() { return (m_currTok < m_tokens.size()); }

std::shared_ptr<Token> Lexer::getToken() { return m_tokens[m_currTok++]; }

void Lexer::debug()
{
  for(auto token : m_tokens)
  {
    std::cout << token->getLineNum() << " - " << token->getScope() << ": " << token->getId() << std::endl;
  }
}


// This whole thing is NASTY! Can I put this all into a throw-away class?
void Lexer::tokenize(const std::string& src)
{
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
        m_tokens.push_back(std::make_shared<Token>(tok_acc, line_num, ws_count));
        tok_acc.clear();
        isOnToken = false;
      }

      ++line_num;
      ws_count = 0;
      mustCountScope = true;
    }
    else if(curr_tok != delim)
    {
      // create token, add to m_tokens
      tok_acc += curr_tok;
      mustCountScope = false;
      isOnToken = true;
    }
    else if(curr_tok == delim)
    {
      if(isOnToken)
      {
        m_tokens.push_back(std::make_shared<Token>(tok_acc, line_num, ws_count));
        tok_acc.clear();
        isOnToken = false;
      }

      if(mustCountScope)
      {
        ++ws_count;
      }
    }
  }
}

void Lexer::eraseComments()
{
  for(int token_index = 0; token_index < m_tokens.size(); token_index++)
  {
    auto token_id = m_tokens[token_index]->getId();

    if(token_id == "//")
    {
      eraseSingleLineComment(token_index);
      --token_index;
    }
    else if (token_id == "/*")
    {
      eraseMultiLineComment(token_index);
      --token_index;
    }
  }
}

void Lexer::eraseSingleLineComment(int start_index)
{
  int comment_line_num = m_tokens[start_index]->getLineNum();
  auto start_comment = m_tokens.begin() + start_index;
  int token_index = start_index;

  while( (token_index < m_tokens.size())
      && (comment_line_num == m_tokens[token_index]->getLineNum()))
  { ++token_index; }

  auto end_comment = m_tokens.begin() + token_index;

  m_tokens.erase(start_comment, end_comment);
}

void Lexer::eraseMultiLineComment(int start_index)
{
  auto start_comment = m_tokens.begin() + start_index;
  int token_index = start_index;

  while( (token_index < m_tokens.size())
      && (m_tokens[token_index]->getId() != "*/"))
  { ++token_index; }

  auto end_comment = m_tokens.begin() + token_index + 1;

  m_tokens.erase(start_comment, end_comment);
}
