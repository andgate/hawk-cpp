#pragma once

#include <fstream>
#include <streambuf>
#include <string>

static std::string load_file(std::string filepath)
{
  std::string str;
  std::ifstream t(filepath);
  
  if(t.is_open())
  {
    t.seekg(0, std::ios::end);
    str.reserve(t.tellg());
    t.seekg(0, std::ios::beg);

    str.assign((std::istreambuf_iterator<char>(t)),
	        std::istreambuf_iterator<char>() );
  }

  return str;
}
