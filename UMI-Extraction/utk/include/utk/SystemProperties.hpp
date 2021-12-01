//
//  SystemProperties.hpp
//  Universal-Toolkit
//
//  Created by Granville Xiong on 9/2/17.
//  Copyright © 2017 Granville Xiong. All rights reserved.
//

#ifndef SystemProperties_hpp
#define SystemProperties_hpp

#include <string>
#include <map>

namespace utk
{

/// \brief Grouped type of operating systems
class OperatingSystem
{
public:
    
    /// \brief Grouped type
    static const std::string type;
    
public:
    
    OperatingSystem() = default;
};

/// \brief Definition of multiple file-system properties
/// FileSystem defines path separator, line delimiter, and user home directory.
class FileSystem
{
public:
    
    /// \brief Table of path separators
    static const std::map<const std::string, const char> path_seps;
    
    /// \brief Table of delimiters of text lines
    static const std::map<const std::string, const char> line_delims;
    
    /// \brief Table of pre-delimiter characters of text lines
    static const std::map<const std::string, const char> pre_delims;
    
    /// \brief Environment variable for path separator in current operating system
    static const char path_sep;
    
    /// \brief Delimiters of text lines in current operating system
    static const char line_delim;
    
    /// \brief Pre-delimiter characters of text lines in current operating system
    static const char pre_delim;
    
    /// \brief User home directory
    static const std::string home_dir;
    
private:
    
    /// \brief Table of environment variables for user home directory
    static const std::map<const std::string, const std::string> home_vars;
    
    /// \brief Environment variable for user home directory in current operating system
    static const std::string home_var;
    
private:
    
    /// \brief Initialize user home directory from environment variable
    static std::string initHomeDir(const std::string& home_var);
    
public:
    
    FileSystem() = default;

    /// \brief Determine if a path exists
    static bool exist(const std::string& path);

    /// \brief Determine if a path is a regular file
    static bool isFile(const std::string& path);

    /// \brief Determine if a path is a directory
    static bool isDir(const std::string& path);
};

}

#endif /* SystemProperties_hpp */
