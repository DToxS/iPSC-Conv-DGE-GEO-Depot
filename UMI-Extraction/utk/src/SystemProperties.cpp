//
//  SystemProperties.cpp
//  Universal-Toolkit
//
//  Created by Granville Xiong on 9/2/17.
//  Copyright © 2017 Granville Xiong. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <sys/stat.h>
#include <utk/SystemProperties.hpp>

namespace utk
{

/// Initialize grouped type of operating systems.
#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64)

/// Windows group.
const std::string OperatingSystem::type {"windows"};

#elif defined(__APPLE__) || defined(__MACH__) || defined(__gnu_linux__) || defined(__linux__) || defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__)

/// Unix group: Unix, Linux, BSD, Mac OS X and later.
const std::string OperatingSystem::type {"unix"};

#elif defined(macintosh) || defined(Macintosh)

/// Classic Macintosh group.
const std::string OperatingSystem::type {"macintosh"};

#else

/// Unknown group.
const std::string OperatingSystem::type {"unknown"};

#endif

/// Initialize table of path separators.
const std::map<const std::string, const char> FileSystem::path_seps = { {"windows",'\\'}, {"unix",'/'}, {"macintosh",'/'}, {"unknown",'\0'} };
/// Initialize table of delimiters of text lines.
const std::map<const std::string, const char> FileSystem::line_delims = { {"windows",'\n'}, {"unix",'\n'}, {"macintosh",'\r'}, {"unknown",'\0'} };
/// Initialize table of pre-delimiter characters of text lines.
const std::map<const std::string, const char> FileSystem::pre_delims = { {"windows",'\r'}, {"unix",'\0'}, {"macintosh",'\0'}, {"unknown",'\0'} };
/// Initialize table of environment variables for user home directory
const std::map<const std::string, const std::string> FileSystem::home_vars = { {"windows","USERPROFILE"}, {"unix","HOME"}, {"macintosh","HOME"}, {"unknown",""} };

/// Initialize path separator in current operating system
const char FileSystem::path_sep {FileSystem::path_seps.at(OperatingSystem::type)};
/// Initialize delimiter of text lines in current operating system
const char FileSystem::line_delim {FileSystem::line_delims.at(OperatingSystem::type)};
/// Initialize pre-delimiter character of text lines in current operating system
const char FileSystem::pre_delim {FileSystem::pre_delims.at(OperatingSystem::type)};
/// Initialize environment variable for user home directory in current operating system
const std::string FileSystem::home_var {FileSystem::home_vars.at(OperatingSystem::type)};

/// Initialize user home directory in current operating system
const std::string FileSystem::home_dir{initHomeDir(FileSystem::home_var)};

/// Initialize user home directory from environment variable
std::string FileSystem::initHomeDir(const std::string& home_var_arg)
{
    std::string local_home_dir;
    if(!home_var_arg.empty())
    {
        // Obtain home directory from operating system.
        const char* const home_dir_cstr = std::getenv(home_var_arg.c_str());
        if(home_dir_cstr != nullptr) local_home_dir = home_dir_cstr;
        else std::cerr << "Environment variable for user home directory " << home_var_arg << " doesn't exist" << std::endl;
    }
    else std::cerr << "Empty environment variable for user home directory" << std::endl;
    return local_home_dir;
}

/// Determine if a given path exists.
bool FileSystem::exist(const std::string& path)
{
    struct stat stat_info;
    return (stat(path.c_str(), &stat_info) == 0);
}

/// Determine if a given path is a regular file.
bool FileSystem::isFile(const std::string& path)
{
    struct stat stat_info;
    return ((stat(path.c_str(), &stat_info) == 0) && (stat_info.st_mode & S_IFREG));
}

/// Determine if a given path is a directory.
bool FileSystem::isDir(const std::string& path)
{
    struct stat stat_info;
    return ((stat(path.c_str(), &stat_info) == 0) && (stat_info.st_mode & S_IFDIR));
}

}
