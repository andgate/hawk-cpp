#include "Nest.h"

Nest::Nest()
  : m_modules()
{}


std::shared_ptr<NestConf> find()
{
	return nullptr;
}

std::shared_ptr<Nest> build(std::shared_ptr<NestConf> nest_conf)
{
	return nullptr;
}

std::shared_ptr<Nest> Nest::fetch()
{
	return build(find());
}
