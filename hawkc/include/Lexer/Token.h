#pragma once

#include <memory>
#include <string>
#include <vector>

class Token
{
public:
  Token(const std::string& tok, int line_num, int m_line_index, int scope, int scope_index);
  Token(const Token& rhs);
  Token(Token&& rhs);

  Token& operator=(const Token& rhs);
  Token& operator=(Token&& rhs);

  std::string getId();
  int get_line_num();
  int get_line_index();
  int get_scope();
  int get_scope_index();

private:
  std::string m_idStr;
  int m_line_num;
  int m_line_index;
  int m_scope;
  int m_scope_index;
};

typedef std::vector<std::shared_ptr<Token>> token_vector;
typedef std::shared_ptr<token_vector> shared_token_vector;