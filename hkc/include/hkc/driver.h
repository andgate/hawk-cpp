#ifndef _HKC_DRIVER_H_
#define _HKC_DRIVER_H_

#include <string>
#include <map>
#include <memory>

#include "hkc/ast.h"

# include "parser.hpp"

// Tell Flex the lexer's prototype ...
# define YY_DECL \
yy::hawk_parser::symbol_type yylex (hawk_driver& driver)
// ... and declare it for the parser's sake.

YY_DECL;
// Conducting the whole scanning and parsing of Calc++.


class hawk_driver
{
public:
    ast::pModule result;
    
    std::string filename;
    bool trace_scanning;
    bool trace_parsing;
    
    hawk_driver();
    
    void scan_begin();
    void scan_end();
    
    bool parse(const std::string& f);
    void reset();
    
    // Error handling.
    void error (const yy::location& l, const std::string& m);
    void error (const std::string& m);
};

#endif