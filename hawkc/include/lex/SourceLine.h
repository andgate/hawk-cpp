#pragma once

#include <string>
#include <vector>
using namespace std;

class SourceLine
{
public:
    SourceLine(const SourceLine*);
    SourceLine(SourceLine&&);
    SourceLine(const string&, const int& line_num);

    SourceLine& operator=(const SourceLine*);
    SourceLine& operator=(SourceLine&&);

    string toString();

private:
    vector<string> m_tokens;
    int m_scope;
    int m_line_num;
};
