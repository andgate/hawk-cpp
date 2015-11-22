#include "lex/SourceLine.h"
#include <Whitespace.h>
#include <boost/algorithm/string.hpp>

SourceLine::SourceLine(const SourceLine* rhs)
    : m_tokens(rhs->m_tokens)
    , m_line_num(rhs->m_line_num)
    , m_scope(rhs->m_scope)
{}

SourceLine::SourceLine(SourceLine&& rhs)
    : m_tokens(move(rhs.m_tokens))
    , m_line_num(move(rhs.m_line_num))
    , m_scope(move(rhs.m_scope))
{}

SourceLine::SourceLine(const string& line, const int& line_num)
    : m_tokens(line)
    , m_line_num(line_num)
    , m_scope(whitespace::lcount(line))
{}

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
