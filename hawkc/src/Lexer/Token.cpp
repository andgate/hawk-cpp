#include "Lexer/Token.h"

Token::Token(const std::string& tok, int line_num, int scope)
  : m_idStr(tok), m_line_num(line_num), m_scope(scope)
{}

Token::Token(const Token& rhs)
  : m_idStr(rhs.m_idStr), m_line_num(rhs.m_line_num), m_scope(rhs.m_scope)
{}

Token::Token(Token&& rhs)
  : m_idStr(std::move(rhs.m_idStr)), m_line_num(std::move(rhs.m_line_num)), m_scope(std::move(rhs.m_scope))
{}

Token& Token::operator=(const Token& rhs)
{
  m_idStr = rhs.m_idStr;
  m_line_num = rhs.m_line_num;
  m_scope = rhs.m_scope;

  return *this;
}

Token& Token::operator=(Token&& rhs)
{
  m_idStr = std::move(rhs.m_idStr);
  m_line_num = std::move(rhs.m_line_num);
  m_scope = std::move(rhs.m_scope);

  return *this;
}

std::string Token::getId()
{
  return m_idStr;
}

int Token::getLineNum()
{
  return m_line_num;
}

int Token::getScope()
{
  return m_scope;
}
