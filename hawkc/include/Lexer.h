#pragma once

#include <memory>
#include <string>
#include <tuple>
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
    string m_line;
    int m_scope;
    int m_line_num;
};

class SourceFile
{
public:
    void addLine(const string& line, const int& line_num);

    string toString();
private:
    vector<shared_ptr<SourceLine>> m_srcLines;
};
