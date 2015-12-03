#include "AST/Module.h"

ModuleAST::ModuleAST(const std::string& file_path, shared_token_vector tokens)
	: m_file_path(file_path)
	, m_tokens(tokens)
	, m_asts()
	, m_imports()
	, m_exports()
	, m_sym_table()
{}

ModuleAST::ModuleAST(const ModuleAST& rhs)
	: m_file_path(rhs.m_file_path)
	, m_tokens(rhs.m_tokens)
	, m_asts(rhs.m_asts)
	, m_imports(rhs.m_imports)
	, m_exports(rhs.m_exports)
	, m_sym_table(rhs.m_sym_table)
{}

ModuleAST::ModuleAST(ModuleAST&& rhs)
	: m_file_path(std::move(rhs.m_file_path))
	, m_tokens(std::move(rhs.m_tokens))
	, m_asts(std::move(rhs.m_asts))
	, m_imports(std::move(rhs.m_imports))
	, m_exports(std::move(rhs.m_exports))
	, m_sym_table(std::move(rhs.m_sym_table))
{}

ModuleAST& ModuleAST::operator=(const ModuleAST& rhs)
{
	m_file_path = rhs.m_file_path;
	m_tokens = rhs.m_tokens;
	m_asts = rhs.m_asts;
	m_imports = rhs.m_imports;
	m_exports = rhs.m_exports;
	m_sym_table = rhs.m_sym_table;
	
	return *this;
}

ModuleAST& ModuleAST::operator=(ModuleAST&& rhs)
{
	m_file_path = std::move(rhs.m_file_path);
	m_tokens = std::move(rhs.m_tokens);
	m_asts = std::move(rhs.m_asts);
	m_imports = std::move(rhs.m_imports);
	m_exports = std::move(rhs.m_exports);
	m_sym_table = std::move(rhs.m_sym_table);

	return *this;
}

shared_token_vector ModuleAST::get_tokens()
{
	return m_tokens;
}

void ModuleAST::add_export(const std::string& export_name)
{
	// validate name?
	if (export_name.empty())
	{
		// silently fail
		// maybe thow exception?
		// all exports should have a name
		return;
	}

	m_exports.push_back(export_name);
}