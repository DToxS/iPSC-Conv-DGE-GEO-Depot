//
//  FileUtils.cpp
//  Universal-Toolkit
//
//  Created by Yuguang Xiong on 9/26/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utk/FileUtils.hpp>
#include <utk/StringUtils.hpp>
#include <utk/SystemProperties.hpp>

namespace utk
{

/// Check if a file can be read.
bool isFileReadable(const std::string& file_path)
{
    return std::ifstream(file_path).is_open();
}

/// Check if a file can be read.
void checkFileReadability(const std::string& file_path)
{
    if(!std::ifstream(file_path).is_open())
    {
        std::stringstream str;
        str << file_path << " cannot be read";
        throw std::runtime_error(str.str());
    }
}

/// Extract file name and directory from a file path.
std::tuple<std::string, std::string> extractFileNameDirectory(const std::string& file_path)
{
    std::tuple<std::string, std::string> file_name_dir;
    if(file_path.find(FileSystem::path_sep) != std::string::npos)
    {
        auto file_path_parts = splitString(file_path, FileSystem::path_sep);
        auto file_name = file_path_parts.back();
        auto file_dir = file_path.substr(0, file_path.size()-file_name.size()-1);
        file_name_dir = std::make_tuple(file_name, file_dir);
    }
    else file_name_dir = std::make_tuple(file_path, "");
    return file_name_dir;
}

/// Extract main and extended names of a file name.
std::tuple<std::string, std::string> extractFileMainExtNames(const std::string& file_name, char sep)
{
    std::tuple<std::string, std::string> file_main_ext_names;
    if(file_name.find(sep) != std::string::npos)
    {
        // Create the regular expression of sep.
        std::string sep_regex;
        if(sep=='.' || sep=='*' || sep=='+' || sep=='?' || sep=='$') sep_regex = std::string("\\") + sep;
        else sep_regex = std::string(1, sep);
        // Split file name using the regular expression of sep.
        auto file_name_parts = splitString(file_name, sep_regex);
        auto ext_name = file_name_parts.back();
        auto main_name = file_name.substr(0, file_name.size()-ext_name.size()-1);
        file_main_ext_names = std::make_tuple(main_name, ext_name);
    }
    else file_main_ext_names = std::make_tuple(file_name, "");
    return file_main_ext_names;
}

}
