//
//  FileUtils.hpp
//  Universal-Toolkit
//
//  Created by Yuguang Xiong on 9/26/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef FileUtils_hpp
#define FileUtils_hpp

#include <string>

namespace utk
{

/// Check if a file can be read.
bool isFileReadable(const std::string& file_path);

/// Check if a file can be read.
void checkFileReadability(const std::string& file_path);

/// Extract file name and directory from a file path.
std::tuple<std::string, std::string> extractFileNameDirectory(const std::string& file_path);

/// Extract main and extended names of a file name.
std::tuple<std::string, std::string> extractFileMainExtNames(const std::string& file_name, char sep='.');

#endif /* FileUtils_hpp */

}
