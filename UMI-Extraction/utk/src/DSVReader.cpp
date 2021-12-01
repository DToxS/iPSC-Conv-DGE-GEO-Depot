//
//  DSVReader.cpp
//  Universal-Toolkit
//
//  Created by Yuguang Xiong on 9/7/17.
//  Copyright © 2017 Granville Xiong. All rights reserved.
//

#include <iostream>
#include <utk/DSVReader.hpp>

namespace utk
{

DSVReader::DSVReader() : LineReader() {}

DSVReader::DSVReader(const std::string& file_name_arg, const std::string& val_delim_arg, bool header_line_arg, std::size_t n_vals_arg, const std::string& line_delim_type_arg) :
    LineReader(file_name_arg, line_delim_type_arg),
    value_delim{val_delim_arg},
    n_values{n_vals_arg},
    header_line{header_line_arg}
{
    checkHeaderValues();
}

DSVReader::DSVReader(DSVReader&& file) :
    LineReader(std::move(file)),
    value_delim{std::move(file.value_delim)},
    n_values{file.n_values},
    header_line{file.header_line} {}

DSVReader::~DSVReader() noexcept {}

DSVReader& DSVReader::operator=(DSVReader&& file)
{
    if(this != &file)
    {
        LineReader::operator=(std::move(file));
        value_delim = std::move(file.value_delim);
        n_values = file.n_values;
        header_line = file.header_line;
    }
    return *this;
}

/// Check the validity of header line.
void DSVReader::checkHeaderLine()
{
    // Split header line into multiple value field names using delimiter.
    std::string line;
    readLine(line);
    if(!line.empty())
    {
        StringsType header_fields = splitString(line, value_delim);
        if(!header_fields.empty()) n_values = header_fields.size();
        else std::cerr << "Failed to determine the number of values in header line" << std::endl;
    }
    else throw std::runtime_error("Empty header line");
}

/// Open file and initialize parameters.
void DSVReader::open(const std::string& file_name_arg, const std::string& val_delim_arg, bool header_line_arg, std::size_t n_vals_arg, const std::string& line_delim_type_arg)
{
    LineReader::open(file_name_arg, line_delim_type_arg);
    value_delim = val_delim_arg;
    n_values = n_vals_arg;
    header_line = header_line_arg;
    checkHeaderValues();
}

}
