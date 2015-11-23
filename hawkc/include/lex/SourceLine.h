#pragma once

#include <string>
using namespace std;

class SourceLine
{
public:
    SourceLine(const SourceLine&);
    SourceLine(SourceLine&&);
    SourceLine(const string&, const int& line_num);

    SourceLine& operator=(const SourceLine*);
    SourceLine& operator=(SourceLine&&);

    ~SourceLine() { }

    string toString();

    int getScope()
    { return m_scope; }

    string getLine()
    { return m_line; }

private:
    string m_line;
    int m_scope;
    int m_line_num;
};
