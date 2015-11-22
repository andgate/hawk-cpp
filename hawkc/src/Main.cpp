#include <boost/spirit/include/qi.hpp>
#include <Core.h>
#include "lex.h"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// someday, this will return a list of strings or something
void parseFile(const string& filename)
{
    ifstream src_file(filename.c_str());

    if(src_file.is_open())
    {
        int line_num = 0;
        string line;
        SourceFile sf;

        while(getline(src_file, line))
        {
            ++line_num;
            sf.addLine(line, line_num);
        }

        cout << sf.toString();
        src_file.close();
    }
    else cout << "Could not open file: " << filename;
}

int main(int argc, char* argv[])
{
    parseFile("bare.hk");

    return 0;
}
