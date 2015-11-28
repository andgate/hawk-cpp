#include "Nest.h"

#include "Nest/NestConf.h"

Nest::Nest()
  : m_modules()
{}

Nest::Nest(const Nest& rhs)
	: m_modules(rhs.m_modules)
{}

Nest::Nest(Nest&& rhs)
	: m_modules(std::move(rhs.m_modules))
{}

Nest& Nest::operator=(const Nest& rhs)
{
	m_modules = rhs.m_modules;
	return *this;
}

Nest& Nest::operator=(Nest&& rhs)
{
	m_modules = std::move(rhs.m_modules);
	return *this;
}


std::shared_ptr<Nest> Nest::load(const std::string& nest_dir)
{
	auto conf = NestConf::loadFromDir(nest_dir);

	return std::make_shared<Nest>();
}

void Nest::build()
{
}


#include <iostream>

void Nest::debug()
{
	std::cout << "*print debug info*" << std::endl;
}