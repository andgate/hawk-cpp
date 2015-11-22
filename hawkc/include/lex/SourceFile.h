#pragma once

#include "lex/SourceLine.h"

#include <memory>
#include <string>
#include <vector>
using namespace std;

class SourceFile
{
public:
    void addLine(const string& line, const int& line_num);

    string toString();
private:
    vector<shared_ptr<SourceLine>> m_srcLines;
};
