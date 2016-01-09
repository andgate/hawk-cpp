#pragma once

#include <vector>
#include <memory>

namespace hkc
{

enum TokenType {
  tok_eof = -1,

  // commands
  tok_def = -2,
  tok_extern = -3,

  // primary
  tok_identifier = -4,
  tok_number = -5,

  tok_block_start = -6,
  tok_block_end = -7,
  tok_end_statement = -8,
};

struct Token
{
  TokenType type;
  int line_num;
  int line_pos;
  int scope;
};

typedef std::shared_ptr<Token> Token_ptr;
typedef std::vector<Token> Tokens;

};
