//
//  StringUtils.hpp
//  Universal-Toolkit
//
//  Created by Yuguang Xiong on 5/6/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef StringUtils_hpp
#define StringUtils_hpp

#include <string>
#include <vector>
#include <sstream>
#include <utility>
#include <type_traits>

namespace utk
{

/// \brief Split a string using a specified separator
/// Note:
/// 1) separator can include multiple characters as required by the
///    std::regex that uses ECMAScript regular expressions pattern
///    syntax.
/// 2) For a string starting and/or endining with the separator, or
///    containing consecutive separators, empty strings are added to
///    the output list of separated parts.
std::vector<std::string> splitString(const std::string& str, const std::string& sep);
std::vector<std::string> splitString(const std::string& str, const char* sep);
std::vector<std::string> splitString(const std::string& str, char sep);

/// \brief Convert a string to upper case
std::string toUpperString(std::string str);

/// \brief Convert a string to lower case
std::string toLowerString(std::string str);

/// Convert a string to specified data type.
template<typename T>
T convert(const std::string& str)
{
    T arg;
    if constexpr (std::is_same_v<short, T> || std::is_same_v<short int, T> || std::is_same_v<signed short, T> || std::is_same_v<signed short int, T>)
    {
        int val = std::stoi(str);
        arg = val;
        if (arg != val)
        {
            std::stringstream msg;
            msg << "Cannot convert " << str << " to short";
            throw std::out_of_range(msg.str());
        }
    }
    else if constexpr (std::is_same_v<int, T> || std::is_same_v<signed, T> || std::is_same_v<signed int, T>) arg = std::stoi(str);
    else if constexpr (std::is_same_v<long, T> || std::is_same_v<long int, T> || std::is_same_v<signed long, T> || std::is_same_v<signed long int, T>) arg = std::stol(str);
    else if constexpr (std::is_same_v<long long, T> || std::is_same_v<long long int, T> || std::is_same_v<signed long long, T> || std::is_same_v<signed long long int, T>) arg = std::stoll(str);
    else if constexpr (std::is_same_v<unsigned short, T> || std::is_same_v<unsigned short int, T>)
    {
        unsigned long val = std::stoul(str);
        arg = val;
        if (arg != val)
        {
            std::stringstream msg;
            msg << "Cannot convert " << str << " to unsigned short";
            throw std::out_of_range(msg.str());
        }
    }
    else if constexpr (std::is_same_v<unsigned, T> || std::is_same_v<unsigned int, T>)
    {
        unsigned long val = std::stoul(str);
        arg = val;
        if (arg != val)
        {
            std::stringstream msg;
            msg << "Cannot convert " << str << " to unsigned int";
            throw std::out_of_range(msg.str());
        }
    }
    else if constexpr (std::is_same_v<unsigned long, T> || std::is_same_v<unsigned long int, T>) arg = std::stoul(str);
    else if constexpr (std::is_same_v<unsigned long long, T> || std::is_same_v<unsigned long long int, T>) arg = std::stoull(str);
    else if constexpr (std::is_same_v<std::size_t, T>)
    {
        if constexpr (sizeof(std::size_t) == sizeof(unsigned short))
        {
            unsigned long val = std::stoul(str);
            arg = val;
            if (arg != val)
            {
                std::stringstream msg;
                msg << "Cannot convert " << str << " to size_t";
                throw std::out_of_range(msg.str());
            }
        }
        else if constexpr (sizeof(std::size_t) == sizeof(unsigned int))
        {
            unsigned long val = std::stoul(str);
            arg = val;
            if (arg != val)
            {
                std::stringstream msg;
                msg << "Cannot convert " << str << " to size_t";
                throw std::out_of_range(msg.str());
            }
        }
        else if constexpr (sizeof(std::size_t) == sizeof(unsigned long))
        {
            arg = std::stoul(str);
        }
        else if constexpr (sizeof(std::size_t) == sizeof(unsigned long long))
        {
            arg = std::stoull(str);
        }
        else
        {
            std::stringstream msg;
            msg << "Cannot convert " << str << " to size_t";
            throw std::out_of_range(msg.str());
        }
    }
    else if constexpr (std::is_same_v<float, T>) arg = std::stof(str);
    else if constexpr (std::is_same_v<double, T>) arg = std::stod(str);
    else if constexpr (std::is_same_v<long double, T>) arg = std::stold(str);
    else if constexpr (std::is_same_v<bool, T>)
    {
        if(std::string str_lower = toLowerString(str); str_lower == "true") arg = true;
        else if(str_lower == "false") arg = false;
        else
        {
            std::stringstream msg;
            msg << "Cannot convert " << str << " to boolean";
            throw std::logic_error(msg.str());
        }
    }
    else if constexpr (std::is_same_v<char, T>) arg = str[0];
    else if constexpr (std::is_same_v<signed char, T>) arg = static_cast<signed char>(str[0]);
    else if constexpr (std::is_same_v<unsigned char, T>) arg = static_cast<unsigned char>(str[0]);
    else if constexpr (std::is_same_v<std::string, T>) arg = str;
    else throw std::runtime_error("Unsupported data type");
    return arg;
}

/// Convert a string to specified data type.
/// Note: this wrapper function is needed for multiple instances specialized with
/// different data types at the same time.
template<typename T>
void convert(const std::string& str, T& val)
{
    val = convert<T>(str);
}

}

#endif /* StringUtils_hpp */
