#include "hkc/driver.h"
#include "parser.hpp"

#include "hkc/ast_printer.h"

hawk_driver::hawk_driver()
: filename(), trace_scanning (false), trace_parsing (false), print_ast(false)
{}

bool hawk_driver::parse(const std::string& f)
{
    filename = f;
    
    scan_begin();
    yy::hawk_parser parser(*this);
    parser.set_debug_level(trace_parsing);
    int res = parser.parse();
    scan_end ();
    
    if(print_ast)
    {
        ast::Printer printer;
        result->accept(printer);
    }
    
    return !res;
}

void hawk_driver::error(const yy::location& l, const std::string& m)
{
    std::cerr << l << ": " << m << std::endl;
}

void hawk_driver::error(const std::string& m)
{
    std::cerr << m << std::endl;
}

void hawk_driver::reset()
{
    result.reset();
}