#include "hkc/lex.h"

#include <fstream>
#include <iostream>

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>

static void lex_stream(std::ifstream& src_file_in, hkc::ast::Module& module)
{
	std::string src_line;
	int line_num = 1;

	while (std::getline(src_file_in, src_line))
	{
		std::vector<std::string> line_toks;
		
		int scope_num = src_line.size();
		boost::trim_left(src_line);
		scope_num -= src_line.size();

		// Matches tokens to regex patterns,
		// organized by order of precedence.
		std::string char_lit_regex = "'([^'\\\\]|\\\\.)*'";
		std::string string_lit_regex = "\"([^\"\\\\]|\\\\.)*\"";
		std::string single_line_comment = "//.*\n";
		std::string multi_line_comment = "/\\**\\*/";
		std::string paren_regex = "[\\(\\)]{1}";
		std::string word_regex = "[A-Za-z0-9_]+'*";
		std::string symbol_regex = "[!\"#$%&'*+,-./\\\\:;<=>?@[\\\]^_`{|}~]+";
		
		boost::regex re(single_line_comment + "|" + multi_line_comment + "|" + char_lit_regex + "|" + string_lit_regex + "|" + paren_regex + "|" + word_regex + "|" + symbol_regex);
		boost::sregex_token_iterator line_iter(src_line.begin(), src_line.end(), re, 0);
		boost::sregex_token_iterator end_iter;

		int line_tok_num = 1;

		while (line_iter != end_iter)
		{
			auto tok = std::make_shared<hkc::lex::Token>();
			tok->tok = line_iter->str();
			tok->tok_num = module.tokens->size();
			tok->line_num = line_num;
			tok->line_tok_num = line_tok_num;
			tok->scope_num = scope_num;

			module.tokens->push_back(tok);

			++line_tok_num;
			++line_iter;
		}

		++line_num;
	}
}

hkc::ast::Module_Vector_sptr hkc::lex::lex(std::vector<std::string> src_files)
{
	auto modules = std::make_shared<hkc::ast::Module_Vector>();

	for (auto src_file : src_files)
	{
		// Open stream to tokens
		std::ifstream src_file_in(src_file, std::ifstream::binary);

		hkc::ast::Module_sptr module = std::make_shared<ast::Module>();
		module->file_path = src_file;
		modules->push_back(module);

		lex_stream(src_file_in, *module);
		src_file_in.close();
	}

	return modules;
}

/*void hkc::lex::debug(const Token_Vector& tokens)
{
  std::cout << "Lexer produced the following tokens at the given line number - scope..." << std::endl;
  for(auto token : tokens)
  {
    std::cout
		<< "(" << token->get_line_num() << "," << token->get_line_index() << ") "
		<< " - " << "(" << token->get_scope() << "," << token->get_scope_index()
		<< ": " << token->getId() << std::endl;
  }
}*/
