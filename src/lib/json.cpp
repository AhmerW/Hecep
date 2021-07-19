#include "../include/json.hpp"
#include "../include/const.hpp"
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include <iostream>

// Json::Json

Json::Json::Json(std::map<std::string, std::string> _data, std::map<std::string, valueTypes> _types) : _data(_data), _types(_types){};

std::string Json::Json::getString()
{
    // TODO: implement recursion
    std::string json = "";
    json += CHR_STRT;

    int i = 1;
    int len = _data.size();
    for (auto const &item : _data)
    {

        std::string temp = "\"" + item.first + "\": ";
        valueTypes _type = _types[item.first];

        if (_type == Array)
        {
            temp += CHR_ARR_STRT + item.second + CHR_ARR_END;
        }
        else if (_type == CHR_STR)
        {
            temp += "\"" + item.second + "\"";
        }
        else
        {
            temp += item.second;
        }

        if (i == len + 1)
        {
            temp += CHR_END;
            break;
        }
        else
        {
            temp += CHR_DELIM;
        }

        json += temp;
        i++;
    }
    json += CHR_END;
    return json;
}

// Json::*

bool Json::isJsonString(std::string raw)
{
    // its a json-string if it starts with  { and ends with }
    // for validating the json, look into validateJsonString()

    return raw.rfind(CHR_STRT, 0) == 0 && ((char)raw[raw.size() - 1] == CHR_END);
}

Json::valueTypes getType(std::string str)
{
    Json::valueTypes _type = Json::Int;
    if (str.find(CHR_DELIM) != std::string::npos)
        return Json::Array;
    char *p;
    long _ = strtol(str.c_str(), &p, 10);
    if (!(*p))
    {
        return Json::Int;
    }
    else
    {
        return Json::String;
    }

    return _type;
}

Json::Json Json::parse(std::string raw)
{
    const std::string _ERR = "Invalid Json";

    int ist = raw.rfind(CHR_STRT, std::string::npos);
    int ien = raw.rfind(CHR_END, std::string::npos) + 1;

    if (ist > raw.size() || ien > raw.size())
    {
        throw std::invalid_argument(_ERR);
    }
    std::string str = raw.substr(ist, ien);
    if (!isJsonString(str))
    {
        throw std::invalid_argument(_ERR);
    }

    const int len = str.size();
    std::vector<std::string> items;

    // First string index
    int f_si = 0;

    int f_li = 0;
    int f_vi = 0;

    for (int i = 1; i < len; i++)
    {

        char c = str[i];

        // arr
        if (c == CHR_ARR_END)
        {
            if (f_li != 0)
            {
                items.push_back(str.substr(f_li + 1, i - f_li - 1));
                f_li = 0;
            }
            else
            {
                throw std::invalid_argument(_ERR);
            }
        }

        // value
        if (c == CHR_DELIM || c == CHR_END)
        {
            if (f_vi != 0)
            {
                items.push_back(str.substr(f_vi, i - f_vi));
                f_vi = 0;
            }
        }
        // string
        if (c == CHR_STR || c == CHR_CHR)
        {

            if (f_si == 0)
            {
                f_si = i;
                continue;
            }
            else
            {
                items.push_back(str.substr(f_si + 1, i - f_si - 1));
                f_si = 0;
                continue;
            }
        }
        // start of arr/value
        if (c == CHR_SEP)
        {
            if (f_vi == 0)
            {
                if (isspace(str[i + 1]))
                {
                    if (str[i + 2] == CHR_ARR_STRT)
                    {
                        f_li = i + 2;
                    }
                    else
                    {
                        f_vi = i + 2;
                    }
                }
                else
                {
                    if (str[i + 1] == CHR_ARR_STRT)
                    {
                        f_li = i + 1;
                    }
                    else
                    {
                        f_vi = i + 1;
                    }
                }
            }
        }
    }
    std::map<std::string, valueTypes> _types;
    std::map<std::string, std::string> _data;

    bool is_key = true;
    for (int i = 0; i < items.size(); i++)
    {
        std::string item = items[i];

        if (is_key)
        {
            is_key = false;
            continue;
        }
        is_key = true;
        _data[items[i - 1]] = item;
        _types[items[i - 1]] = getType(item);
    }
    if (_data.size() != _types.size())
    {
        throw std::invalid_argument(_ERR);
    }

    return Json(_data, _types);
}
