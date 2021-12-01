//
//  StringUtils.cpp
//  Universal-Toolkit
//
//  Created by Yuguang Xiong on 5/6/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#include <regex>
#include <cctype>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <utk/StringUtils.hpp>

namespace utk
{

/// Split a string using a specified separator.
std::vector<std::string> splitString(const std::string& str, const std::string& sep)
{
    std::vector<std::string> parts;
    if(!str.empty())
    {
        if(sep.length() > 0)
        {
            if(sep != "|")
            {
                std::regex sep_regex(sep);
                std::sregex_token_iterator sregex_end;
                for(auto part_it=std::sregex_token_iterator(str.begin(), str.end(), sep_regex, -1); part_it!=sregex_end; part_it++)
                {
                    parts.push_back(*part_it);
                }
                // Add an empty part if the string ends with separator.
                if(std::regex_search(str, std::regex(sep+'$'))) parts.push_back(std::string());
            }
            else throw std::logic_error("Character | must be escaped");
        }
        else parts.push_back(str);
    }
    return parts;
}

/// Split a string using a specified separator.
std::vector<std::string> splitString(const std::string& str, const char* sep)
{
    return splitString(str, std::string(sep));
}

/// Split a string using a specified separator.
std::vector<std::string> splitString(const std::string& str, char sep)
{
    return splitString(str, std::string(1,sep));
}

/// Convert a string to upper case.
std::string toUpperString(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){return std::toupper(c);});
    return str;
}

/// Convert a string to lower case.
std::string toLowerString(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c){return std::tolower(c);});
    return str;
}

}
