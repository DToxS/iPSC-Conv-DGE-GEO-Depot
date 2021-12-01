//
//  LineReader.cpp
//  Universal-Toolkit
//
//  Created by Yuguang Xiong on 9/7/17.
//  Copyright © 2017 Granville Xiong. All rights reserved.
//

#include <utility>
#include <sstream>
#include <iostream>
#include <utk/DSVReader.hpp>
#include <utk/SystemProperties.hpp>

namespace utk
{

/// \brief Cross-platform line reader for text file
/// This class reads in a line from a text file and removes all line delimiter
/// characters:
///     Unix: \n
///     Windows: \r\n
///     Classic Macintosh: \r
    
LineReader::LineReader() : std::ifstream() {}

LineReader::LineReader(const std::string& file_name_arg, const std::string& line_delim_type_arg) :
    std::ifstream(file_name_arg),
    file_name{file_name_arg},
    line_delim_type{line_delim_type_arg},
    line_delim{widen(FileSystem::line_delims.at(line_delim_type))},
    pre_delim{widen(FileSystem::pre_delims.at(line_delim_type))}
{
    checkFileOpen();
}

LineReader::LineReader(LineReader&& file) :
    std::ifstream(std::move(file)),
    file_name{std::move(file.file_name)},
    line_delim_type{std::move(file.line_delim_type)},
    line_delim{file.line_delim},
    pre_delim{file.pre_delim},
    file_end{file.file_end},
    read_failed{file.read_failed}
{
    file.reset();
}

LineReader::~LineReader() noexcept {}

LineReader& LineReader::operator=(LineReader&& file)
{
    if(this != &file)
    {
        std::ifstream::operator=(std::move(file));
        file_name = std::move(file.file_name);
        line_delim_type = std::move(file.line_delim_type);
        line_delim = file.line_delim;
        pre_delim = file.pre_delim;
        file_end = file.file_end;
        read_failed = file.read_failed;
        file.reset();
    }
    return *this;
}

/// Check if input file is open.
void LineReader::checkFileOpen()
{
    if(!is_open())
    {
        std::stringstream str;
        str << "Cannot open input file " << file_name << '!';
        throw std::runtime_error(str.str());
    }
}

/// Clear all data member about file contents.
void LineReader::reset()
{
    // Clear data members about file contents.
    file_name.clear();
    line_delim_type.clear();
    line_delim = '\0';
    pre_delim = '\0';
    // Do NOT call resetStream to reset input stream because it's a common
    // system resource so that its change will affect all the objects that
    // operate it.
}

/// Open file and initialize parameters.
void LineReader::open(const std::string& file_name_arg, const std::string& line_delim_type_arg)
{
    file_name = file_name_arg;
    std::ifstream::open(file_name);
    checkFileOpen();
    line_delim_type = line_delim_type_arg;
    line_delim = widen(FileSystem::line_delims.at(line_delim_type));
    pre_delim = widen(FileSystem::pre_delims.at(line_delim_type));
}

/// Close file.
void LineReader::close()
{
    // Close input file stream.
    std::ifstream::close();
    if(fail()) std::cerr << "Error occurred when closing the file " << file_name << " and ignore it!" << '\n';
    // Clear all error state flags of input stream.
    std::ios::clear();
    // Reset read flags of LineReader to initial state.
    resetIOFlags();
    /// Clear all data member about file contents.
    reset();
}

/// Read a text line and remove line delimiters.
bool LineReader::readLine(std::string& line)
{
    // Read a line.
    bool status = static_cast<bool>(std::getline(*this, line, line_delim));

    // Remove non-empty pre-delim character if the type of line delimiter
    // is different from the type of current operating system.
    if(status && !line.empty() && pre_delim != '\0' && line_delim_type != OperatingSystem::type) line.pop_back();

    // Set the flag if file end is reached.
    if(eof()) file_end = true;

    // Set the flag if read operation fails.
    if(fail()) read_failed = true;

    return status;
}

/// Read multiple text lines.
LineReader::LinesType LineReader::readLines(std::size_t n_lines)
{
    // Initialize lines.
    LinesType lines;

    // Start reading multiple lines.
    if(!file_end)
    {
        if(n_lines > 0)
        {
            // Read specified number of lines.
            for(std::size_t counts = 0; counts < n_lines; ++counts)
            {
                if(std::string line; readLine(line)) lines.push_back(std::move(line));
                else break;
            }
        }
        else
        {
            // Read all lines.
            for(std::string line; readLine(line);) lines.push_back(std::move(line));
        }
    }

    return lines;
}

}
