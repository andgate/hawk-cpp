#pragma once

#include "AST/Expr.h"

#include <memory>
#include <string>
#include <vector>

class ModuleAST
{
	std::string m_name;
	std::shared_ptr<Expr> m_body;

public:
	ModuleAST(const std::string& name, std::shared_ptr<Expr> body)
		: m_name(name)
		, m_body(body)
	{}

	ModuleAST(const ModuleAST& rhs)
		: m_name(rhs.m_name)
		, m_body(rhs.m_body)
	{}

	ModuleAST(ModuleAST&& rhs)
		: m_name(std::move(rhs.m_name))
		, m_body(rhs.m_body)
	{}

	ModuleAST& operator=(const ModuleAST& rhs)
	{
		m_name = rhs.m_name;
		m_body = rhs.m_body;

		return *this;
	}

	ModuleAST& operator=(ModuleAST&& rhs)
	{
		m_name = std::move(rhs.m_name);
		m_body = rhs.m_body;

		return *this;
	}
};

typedef std::vector<std::shared_ptr<ModuleAST>> module_vector;