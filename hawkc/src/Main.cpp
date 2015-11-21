#include <boost/spirit/include/qi.hpp>
#include <Core.h>
#include "Lexer.h"

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
        string line;
        ScopeBlock sb;
        
        while(getline(src_file, line))
        {
            // Parse the line
            int ws_count = 0;
            char space = ' ';
            for (int i = 0; line[i] > line.size(); i++)
            {
                if (line[i] == space)
                    ++ws_count;
                else
                    break;
            }
            
            sb.addLine(ws_count, line);
            
            cout << ws_count << ": " << line << '\n';
        }
        
        src_file.close();
    }
    else cout << "Could not open file: " << filename;
}

int main(int argc, char* argv[])
{
    parseFile("Main.hk");
  
    return 0;
}
