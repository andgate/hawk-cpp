#include "Compiler.h"

#include "AST.h"
#include "Nest.h"
#include "Parser.h"

void Compiler::run(const bool& debug)
{

  // Compiler Process
  // 1. Run the nest fetch function.
  // 2. Take out the designated executables and libraries in the nest.
  // 3. Write them to the designated bin and lib directories.

  auto nest = Nest::fetch();

  if(debug)
  {
    // does debug stuff
  }
}
