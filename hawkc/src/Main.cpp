#include <boost/spirit/include/qi.hpp>
#include <Core.h>

int main(int argc, char* argv[])
{
  int n = testFunc();
  std::cout << "Result: " << n << std::endl;
  return 0;
}
