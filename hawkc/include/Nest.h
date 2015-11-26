#pragma once

#include "AST.h"

#include <memory>

class Nest
{
public:
  typedef std::vector<std::shared_ptr<ModuleAST>> module_vector;

  Nest();

  static std::shared_ptr<Nest> fetch();

private:
  module_vector m_modules;
};
