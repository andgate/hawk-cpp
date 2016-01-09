#include "hkc/parser/Lexer.h"

#include "hkc/utils/ReadFile.h"

#include "hkc/parser/Token.h"

// Used to tokenize the entire source base
std::vector<hkc::Tokens> hkc::lex_files(std::vector<std::string> src_paths)
{
  std::vector<hkc::Tokens> token_vecs;
  
  // Load each file into memory and lex it entirely.
  // This is primitive, and can exhaust memory for large scale projects.
  // However, for now, this method will suffice.
  for(auto src_path : src_paths)
  {
    auto src = load_file(src_path);

    // Feed stream into lexer
    auto lexer = Lexer(src, src_path);
    token_vecs.push_back(lexer.get_toks());
  }
  
  return token_vecs;
}

hkc::Lexer::Lexer(std::string src, std::string src_path)
  : m_src(src)
  , m_src_path(src_path)
  , m_pos(0)
  , m_line_num(0)
  , m_col_num(0)
  , states()
{
  
}

hkc::Lexer::~Lexer()
{
  // Nothing to delete so far
}

hkc::Tokens hkc::Lexer::get_toks()
{
  std::vector<hkc::Token> toks;
  return toks;
}
