#pragma once

#include "lex/SourceLine.h"

#include <memory>
#include <string>
using namespace std;

#include <boost/ptr_container/ptr_vector.hpp>

class SourceFile
{
public:
    void addLine(const string& line, const int& line_num);

    SourceLine operator[](const int& n);

    int size();

    string toString();
private:
    boost::ptr_vector<SourceLine> m_srcLines;
};
