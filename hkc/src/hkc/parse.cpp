#include "hkc/parse.h"

#include <iostream>
#include <memory>

const char EXPORT_SYMBOL = '<';
const char IMPORT_SYMBOL = '>';

char OPTION_SYMBOL = '@';
char TYPEDEC_SYMBOL = ':';
char DEC_SYMBOL = '=';

static void parse_top_level(hkc::Ast_sptr& ast);
static void parse_top_level_module(hkc::ast::Module_sptr ast);
static void parse_top_level_imports(hkc::Ast_sptr& ast);
static void parse_top_level_config(hkc::Ast_sptr& ast);
static void parse_top_level_typesig(hkc::Ast_sptr& ast);
static void parse_top_level_function(hkc::Ast_sptr& ast);

hkc::Ast_sptr hkc::parse(ast::Module_Vector_sptr modules)
{
	auto module_ast = std::make_shared<Ast>();

	module_ast->modules = *modules;

	parse_top_level(module_ast);

	return module_ast;
}

// Builds modules dependency graph
// and symbol table.
void parse_top_level(hkc::Ast_sptr& ast)
{
	for(auto module : ast->modules)
	{
		parse_top_level_module(module);
	}
}


std::string parse_module_name(hkc::lex::Token_Vector_sptr& tokens, int token_i, hkc::lex::Token_sptr& token)
{
	std::string module_name;
	auto curr_token = (*tokens)[token_i];

	while(curr_token)
	{

	}


	return module_name;
}

void parse_top_level_module(hkc::ast::Module_sptr module)
{
	auto tokens = module->tokens;
	for (int token_i = 0; token_i < tokens->size(); token_i++)
	{
		auto key_token = (*tokens)[token_i];

		if(key_token->tok == "<")
		{
			int line_num = key_token->line_num;
			int scope = key_token->scope_num;

			tokens->erase(tokens->begin() + token_i);

			auto curr_token = (*tokens)[token_i];
			while (key_token->line_num == curr_token->line_num
				|| key_token->scope_num < curr_token->scope_num);
			{
				if(curr_token->tok != ".")
				{
					auto export_name = parse_module_name(tokens, token_i, key_token);
					module->exports.push_back(export_name);
				}
			}

		}
	}
}


void parse_top_level_exports(hkc::Ast_sptr& ast)
{

}

void parse_top_level_imports(hkc::Ast_sptr& ast)
{}

void parse_top_level_config(hkc::Ast_sptr& ast)
{}

void parse_top_level_typesig(hkc::Ast_sptr& ast)
{}

void parse_top_level_function(hkc::Ast_sptr& ast)
{}



/*void hkc::debug()
{
  std::cout << std::endl;
  std::cout << "Parser Produced the following output" << std::endl;
}*/

/*static Expr parseNumericExpression(const token_vector& tokens)
{
  return Exprb(0);
}*/


/*static Expr parseProtoExpression(const token_vector& tokens)
{
  for(int i = 0; i < tokens.size(); i++)
  {
    auto token { tokens[i] };

    if(token == "=")
    {

    }
  }

  //const std::string &name, std::vector<std::string> Args

  return PrototypeAST(tokens);
}*/

/*static token_vector parseTypeSigStr(const int& token_index, const token_vector& tokens)
{
  int scope = tokens[token_index - 1]->get_scope();
  int lineNum = tokens[token_index - 1]->get_line_num();

  token_vector out_tokens;

  for(int token_index_offset = 1; (token_index_offset+token_index) < tokens.size(); token_index_offset++)
  {
    auto curr_token = tokens[token_index + token_index_offset];

    if(curr_token->getId() != "->")
    {
      if(curr_token->get_line_num() == lineNum)
      {
        out_tokens.push_back(curr_token);
      } else if(curr_token->get_scope() > scope)
      {
        out_tokens.push_back(curr_token);
      }
      else
      {
        return out_tokens;
      }
    }
  }
}

static std::shared_ptr<TypeSigAST> parseTypeSigAST(const int& token_index, const token_vector& tokens)
{
  auto name = tokens[token_index - 1];
  auto args = parseTypeSigStr(token_index, tokens);

  return std::make_shared<TypeSigAST>(TypeSigAST(name, args));
}*/

static std::unique_ptr<hkc::ast::Module> parseModule(std::unique_ptr<hkc::ast::Module> module_ptr)
{
	// Check the module expr to see is unparsed.
	// If so, then need to scan tokens for module name,
	// and module name definition conflicts.
	// Once module name is adjusted correctly, parsing begins.


	return module_ptr;
}
