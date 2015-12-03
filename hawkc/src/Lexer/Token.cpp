#include "Lexer/Token.h"

Token::Token(const std::string& tok, int line_num, int line_index, int scope, int scope_index)
	: m_idStr(tok), m_line_num(line_num), m_line_index(line_index), m_scope(scope), m_scope_index(scope_index)
{}

Token::Token(const Token& rhs)
	: m_idStr(rhs.m_idStr)
	, m_line_num(rhs.m_line_num)
	, m_line_index(rhs.m_line_index)
	, m_scope(rhs.m_scope)
	, m_scope_index(rhs.m_scope_index)
{}

Token::Token(Token&& rhs)
	: m_idStr(std::move(rhs.m_idStr))
	, m_line_num(std::move(rhs.m_line_num))
	, m_line_index(std::move(rhs.m_line_index))
	, m_scope(std::move(rhs.m_scope))
	, m_scope_index(std::move(rhs.m_scope_index))
{}

Token& Token::operator=(const Token& rhs)
{
  m_idStr = rhs.m_idStr;
  m_line_num = rhs.m_line_num;
  m_line_index = rhs.m_line_index;
  m_scope = rhs.m_scope;
  m_scope_index = rhs.m_scope_index;

  return *this;
}

Token& Token::operator=(Token&& rhs)
{
  m_idStr = std::move(rhs.m_idStr);
  m_line_num = std::move(rhs.m_line_num);
  m_line_index = std::move(rhs.m_line_index);
  m_scope = std::move(rhs.m_scope);
  m_scope_index = std::move(rhs.m_scope_index);

  return *this;
}

std::string Token::getId()
{
  return m_idStr;
}

int Token::get_line_num()
{
  return m_line_num;
}

int Token::get_line_index()
{
	return m_line_index;
}

int Token::get_scope()
{
  return m_scope;
}

int Token::get_scope_index()
{
	return m_scope_index;
}
