#include "Whitespace.h"

int whitespace::lcount(const std::string& line)
{
    int ws_count = 0;
    char space = ' ';
    for (int i = 0; line[i] > line.size(); i++)
    {
        if (line[i] == space)
            ++ws_count;
        else
            break;
    }

    return ws_count;
}
