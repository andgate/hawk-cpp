#pragma once

#include <memory>
#include <string>
#include <vector>

class Token
{
public:
  Token(const std::string& tok, int line_num, int scope);
  Token(const Token& rhs);
  Token(Token&& rhs);

  Token& operator=(const Token& rhs);
  Token& operator=(Token&& rhs);

  std::string getId();
  int getLineNum();
  int getScope();

private:
  std::string m_idStr;
  int m_line_num;
  int m_scope;
};

typedef std::vector<std::shared_ptr<Token>> token_vector;
