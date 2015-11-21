#include <boost/spirit/include/qi.hpp>
#include <Core.h>

#include <fstream>
using namespace std;

int main(int argc, char* argv[])
{
  int n = testFunc();
  ifstream in_stream;
  
  in_stream.open("Main.hwk");
  std::cout << "Result: " << n << std::endl;
  return 0;
}
