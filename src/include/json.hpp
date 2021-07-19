#include "iostream"
#include "string.h"
#include <map>

#ifndef JSON_H
#define JSON_H

namespace Json
{

    // Valid types for json as defined in rfc7159
    enum valueTypes
    {
        String,
        Int,
        Bool,
        Array
    };

    bool isJsonString(std::string);

    class Json
    {
    private:
        std::map<std::string, std::string> _data;
        std::map<std::string, valueTypes> _types;

    public:
        Json(std::map<std::string, std::string> data, std::map<std::string, valueTypes> types);
        std::string getString();
    };

    Json parse(std::string);
}

#endif