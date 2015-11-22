#include "lex/SourceFile.h"
#include <boost/algorithm/string.hpp>

void SourceFile::addLine(const string& line, const int& line_num)
{
    m_srcLines.push_back(make_shared<SourceLine>(new SourceLine(line, line_num)));
}

string SourceFile::toString()
{
    string out;

    for(auto srcLine : m_srcLines)
    {
        out += srcLine->toString();
    }

    return out;
}
