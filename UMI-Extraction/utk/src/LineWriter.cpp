//
//  LineWriter.cpp
//  Universal-Toolkit
//
//  Created by Yuguang Xiong on 3/30/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#include <utility>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <utk/LineWriter.hpp>

namespace utk
{

LineWriter::LineWriter() : std::ofstream() {}

    LineWriter::LineWriter(const std::string& file_name_arg, std::ios::char_type line_delim_arg) : std::ofstream(file_name_arg), file_name{file_name_arg}, line_delim{line_delim_arg}
{
    checkFileOpen();
}

    LineWriter::LineWriter(LineWriter&& file) : std::ofstream(std::move(file)), file_name{std::move(file.file_name)}, line_delim{file.line_delim}, write_failed{file.write_failed}
{
    file.reset();
}

LineWriter::~LineWriter() noexcept {}

LineWriter& LineWriter::operator=(LineWriter&& file)
{
    if(this != &file)
    {
        std::ofstream::operator=(std::move(file));
        file_name = std::move(file.file_name);
        line_delim = file.line_delim;
        write_failed = file.write_failed;
        file.reset();
    }
    return *this;
}

/// Check if output file is open.
void LineWriter::checkFileOpen()
{
    if(!is_open())
    {
        std::stringstream str;
        str << "Cannot open output file " << file_name << '!';
        throw std::runtime_error(str.str());
    }
}

/// Clear all data member about file contents.
void LineWriter::reset()
{
    // Clear data members about file contents.
    file_name.clear();
    /// Clear line delimiter.
    line_delim = '\0';
    // Do NOT call resetStream to reset output stream because it's a common
    // system resource so that its change will affect all the objects that
    // operate it.
}

/// Open file and initialize parameters.
void LineWriter::open(const std::string& file_name_arg)
{
    file_name = file_name_arg;
    std::ofstream::open(file_name);
    checkFileOpen();
}

/// Close file.
void LineWriter::close()
{
    // Close output file stream.
    std::ofstream::close();
    if(fail()) std::cerr << "Error occurred when closing the file " << file_name << " and ignore it!" << '\n';
    // Clear all error state flags of output stream.
    std::ios::clear();
    // Reset write flags of LineReader to initial state.
    resetIOFlags();
    /// Clear all data member about file contents.
    reset();
}

}
