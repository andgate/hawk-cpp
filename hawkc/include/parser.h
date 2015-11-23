#pragma once

#include "lex.h"

#include <boost/spirit/home/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

#include <iostream>
#include <string>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

using qi::double_;
using qi::_1;
using ascii::space;
using phoenix::ref;

namespace hawk
{
    void parse(SourceFile srcFile);

    template <typename Iterator>
    struct parser : qi::grammar<Iterator, unsigned()>
    {
        parser() : parser::base_type(start)
        {
            using ascii::char_;

            start %=
                ascii::string
                >> char_('=')
                >>  ascii::string
                ;
        }

        qi::rule<Iterator, std::string(), ascii::space_type> start;
    };
}
