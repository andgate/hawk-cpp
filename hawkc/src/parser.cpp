#include "parser.h"

void hawk::parse(SourceFile srcFile)
{
    typedef std::string::const_iterator iterator_type;
    //typedef hawk::parser<iterator_type> parser;

    hawk::parser<iterator_type> hawk_parser;

    for(int i = 0; i < srcFile.size(); i++)
    {
        string line = srcFile[0].getLine();

        std::string::const_iterator iter = line.begin();
        std::string::const_iterator end = line.end();
//        bool r = parse(iter, end, hawk_parser, result);
    }
}
