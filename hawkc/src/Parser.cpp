#include "Parser.h"

#include "Lexer.h"

#include <iostream>
#include <memory>

Parser::Parser(shared_module_vector modules)
	: m_modules(modules)
{
	
}

void Parser::operator()()
{
	parse_top_level();
}

const char EXPORT_SYMBOL = '<';
const char IMPORT_SYMBOL = '>';

char OPTION_SYMBOL = '@';
char TYPEDEC_SYMBOL = ':';
char DEC_SYMBOL = '=';

void Parser::parse_top_level()
{
	parse_top_level_exports();
}

void Parser::parse_top_level_exports()
{
	for (auto module : *m_modules)
	{
		auto tokens = module->get_tokens();

		for (int token_i = 0; token_i < tokens->size(); token_i++)
		{
			auto token = (*tokens)[token_i];

			if (token->getId().size() == 0)
			{
				// uh oh, empty tokens SHOULDN'T exists!
				return;
				// return for now...
			}

			if ((token->getId()[0] == EXPORT_SYMBOL))
			{
				if (token->get_scope() == 0 && token->get_scope_index() == 1)
				{
					if (token->getId().size() > 1)
					{
						auto export_name = std::string(token->getId().begin() + 1, token->getId().end());
						module->add_export(export_name);

						auto token_iter = tokens->begin() + token_i;
						tokens->erase(token_iter);
					}
					else if (token->getId().size() == 1)
					{
						auto export_name = (*tokens)[token_i + 1]->getId();
						module->add_export(export_name);

						// delete the current token, use whatever replaces it?
						auto begin = tokens->begin() + token_i;
						auto end = begin + 1;
						tokens->erase(begin, end);
					}
				}
			}
		}
	}
}

void Parser::parse_top_level_imports()
{}

void Parser::parse_top_level_config()
{}

void Parser::parse_top_level_typesig()
{}

void Parser::parse_top_level_function()
{}



void Parser::debug()
{
  std::cout << std::endl;
  std::cout << "Parser Produced the following output" << std::endl;
}

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

static std::unique_ptr<ModuleAST> parseModule(std::unique_ptr<ModuleAST> module_ptr)
{
	// Check the module expr to see is unparsed.
	// If so, then need to scan tokens for module name,
	// and module name definition conflicts.
	// Once module name is adjusted correctly, parsing begins.


	return module_ptr;
}
