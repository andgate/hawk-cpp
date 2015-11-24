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
  Token(const std::string& tok, int line_num, int scope);
  Token(const Token& rhs);
  Token(Token&& rhs);

  Token& operator=(const Token& rhs);
  Token& operator=(Token&& rhs);

  TokenType getType();
  std::string getId();
  int getLineNum();
  int getScope();

private:
  TokenType m_type;
  std::string m_idStr;
  int m_line_num;
  int m_scope;
};
