#include "Lexer.h"

#include <iostream>


static std::shared_ptr<token_vector> tokenize(const std::string& src);
static void eraseComments(token_vector& tokens);

std::shared_ptr<token_vector> Lexer::run(const std::string& src_txt)
{
	auto tokens = tokenize(src_txt);
	eraseComments(*tokens);
	return tokens;
}

std::shared_ptr<token_vector> Lexer::run(std::istream& src_txt_in)
{
	std::string src_txt;
	std::getline(src_txt_in, src_txt, (char)EOF);
	return run(src_txt);
}

void Lexer::debug(const token_vector& tokens)
{
  std::cout << "Lexer produced the following tokens at the given line number - scope..." << std::endl;
  for(auto token : tokens)
  {
    std::cout
		<< "(" << token->get_line_num() << "," << token->get_line_index() << ") "
		<< " - " << "(" << token->get_scope() << "," << token->get_scope_index()
		<< ": " << token->getId() << std::endl;
  }
}


// This whole thing is NASTY! Can I put this all into a throw-away class?
static std::shared_ptr<token_vector> tokenize(const std::string& src)
{
  auto tokens = std::make_shared<token_vector>();

  std::string tok_acc;
  char delim = ' ';

  int line_num = 1;
  int line_index = 0;

  int previous_scope = 0;
  int scope = 0;
  int scope_index = 0;

  bool mustCountScope = true;
  bool is_building_token = false;

  int start_token = 0;
  int end_token = 0;

  for(auto i = 0; i < src.size(); i++)
  {
    auto curr_tok = src[i];

    if(curr_tok == '\n')
    {
      if(is_building_token)
      {
		  ++line_index;
		++scope_index;
		tokens->push_back(std::make_shared<Token>(tok_acc, line_num, line_index, scope, scope_index));
        tok_acc.clear();
		is_building_token = false;
      }

      ++line_num;
	  line_index = 0;
	  previous_scope = scope;
      scope = 0;
      mustCountScope = true;
    }
    else if(curr_tok != delim)
    {
      // create token, add to tokens
      tok_acc += curr_tok;
	  is_building_token = true;

	  if(mustCountScope)
	  {
		  mustCountScope = false;
		  if(scope >= previous_scope)
		  {
			  scope_index = 0;
		  }
	  }
	  
    }
    else if(curr_tok == delim)
    {
		if(is_building_token)
		{
			++scope_index;
			++line_index;
			tokens->push_back(std::make_shared<Token>(tok_acc, line_num, line_index, scope, scope_index));
			tok_acc.clear();
			is_building_token = false;
		}

      if(mustCountScope)
      {
        ++scope;
      }
    }
  }

  return tokens;
}

static void eraseSingleLineComment(token_vector& tokens, const int& start_index)
{
  int comment_line_num = tokens[start_index]->get_line_num();
  auto start_comment = tokens.begin() + start_index;
  int token_index = start_index;

  while( (token_index < tokens.size())
      && (comment_line_num == tokens[token_index]->get_line_num()))
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
