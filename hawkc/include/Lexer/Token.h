#pragma once

#include <memory>
#include <string>

enum TokenType {
  tok_eof = -1,

  // commands
  tok_def = -2,
  tok_extern = -3,

  // primary
  tok_identifier = -4,
  tok_number = -5
};

class Token
{
public:
  Token(const std::string& tok, int line_num, int scope)
    : m_type(tok_eof), m_idStr(tok)
    , m_line_num(line_num), m_scope(scope)
  {}

  Token(const Token& rhs)
    : m_type(rhs.m_type), m_idStr(rhs.m_idStr)
    , m_line_num(rhs.m_line_num), m_scope(rhs.m_scope)
  {}

  Token(Token&& rhs)
    : m_type(std::move(rhs.m_type)), m_idStr(std::move(rhs.m_idStr))
    , m_line_num(std::move(rhs.m_line_num)), m_scope(std::move(rhs.m_scope))
  {}

  Token& operator=(const Token&);
  Token& operator=(Token&&);

  TokenType m_type;
  std::string m_idStr;
  int m_line_num;
  int m_scope;
};
