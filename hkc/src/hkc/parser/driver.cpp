#include "hkc/parser/driver.h"
#include "parser.hpp"

hawk_driver::hawk_driver()
: trace_scanning (false), trace_parsing (false)
{}

hawk_driver::~hawk_driver() {}

int hawk_driver::parse(const std::string &f)
{
    file = f;
    scan_begin();
    yy::hawk_parser parser(*this);
    parser.set_debug_level(trace_parsing);
    int res = parser.parse();
    scan_end ();
    return res;
}

void hawk_driver::error(const yy::location& l, const std::string& m)
{
    std::cerr << l << ": " << m << std::endl;
}

void hawk_driver::error(const std::string& m)
{
    std::cerr << m << std::endl;
}
