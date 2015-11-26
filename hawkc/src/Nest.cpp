#include "Nest.h"

#include "Nest/NestBuilder.h"
#include "Nest/NestFinder.h"

Nest::Nest()
  : m_modules()
{}

std::shared_ptr<Nest> Nest::fetch()
{
  return NestBuilder::build(NestFinder::findInRunDir());
}
