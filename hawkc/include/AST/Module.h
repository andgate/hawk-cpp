#pragma once

#include "AST.h"
#include "Symbol.h"
#include "Lexer/Token.h"

#include <memory>
#include <string>
#include <vector>
#include <map>

#include <boost/any.hpp>

class ModuleAST
{
public:
	ModuleAST(const std::string& file_path, shared_token_vector tokens);

	ModuleAST(const ModuleAST& rhs);

	ModuleAST(ModuleAST&& rhs);

	ModuleAST& operator=(const ModuleAST& rhs);
	ModuleAST& operator=(ModuleAST&& rhs);

	shared_token_vector get_tokens();

	void add_export(const std::string& export_name);

private:
	std::string m_file_path;
	shared_token_vector m_tokens;
	std::vector<boost::any> m_asts;

	std::vector<ModuleAST> m_imports;
	std::vector<std::string> m_exports;

	std::map<std::string, std::vector<Symbol>> m_sym_table;
};

typedef std::vector<std::shared_ptr<ModuleAST>> module_vector;
typedef std::shared_ptr<module_vector> shared_module_vector;