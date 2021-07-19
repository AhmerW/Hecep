#include "include/json.hpp"
#include <iostream>
#include <string>
#include <map>



int main(int argc, char const *argv[])
{
    Json::Json json = Json::parse("{'a': [102, 302], 'b': 10}");
    std::cout << json.getString() << std::endl;
    return 0;
}

// g++ main.cpp lib/*.cpp -Iinclude/ -o out.exe