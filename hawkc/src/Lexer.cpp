#include "Lexer.h"
#include <Whitespace.h>
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

SourceLine::SourceLine(const SourceLine* rhs)
    : m_line(rhs->m_line)
    , m_line_num(rhs->m_line_num)
    , m_scope(rhs->m_scope)
{}

SourceLine::SourceLine(SourceLine&& rhs)
    : m_line(move(rhs.m_line))
    , m_line_num(move(rhs.m_line_num))
    , m_scope(move(rhs.m_scope))
{}

SourceLine::SourceLine(const string& line, const int& line_num)
    : m_line(line)
    , m_line_num(line_num)
    , m_scope(0)
{
    boost::algorithm::trim(m_line);
    m_scope = line.size() - m_line.size();
}

SourceLine& SourceLine::operator=(const SourceLine* rhs)
{
    m_line = rhs->m_line;
    m_line_num = rhs->m_line_num;
    m_scope = rhs->m_scope;
    return *this;
}

SourceLine& SourceLine::operator=(SourceLine&& rhs)
{
    m_line = move(rhs.m_line);
    m_line_num = move(rhs.m_line_num);
    m_scope = move(rhs.m_scope);
    return *this;
}

string SourceLine::toString()
{
    string out;
    out += to_string(m_line_num) + " - " + to_string(m_scope) + ": " + m_line + "\n";
    return out;
}
