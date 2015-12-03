#pragma once

#include "AST.h"

#include <memory>

class Nest
{
public:
  Nest();
  Nest(const Nest& rhs);
  Nest(Nest&& rhs);

  Nest& operator=(const Nest& rhs);
  Nest& operator=(Nest&& rhs);

  static std::shared_ptr<Nest> load(const std::string& nest_dir);
  void build();
  void debug();

private:
	std::string m_name;
	shared_module_vector m_modules;
};
