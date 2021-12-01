//
//  SAMHeaderLine.cpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 3/20/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#include <algorithm>
#include <utility>
#include <sstream>
#include <stdexcept>
#include <utk/StringUtils.hpp>
#include <hts/SAMHeaderLine.hpp>

namespace hts
{

/// Initialization of standard record types of SAM header.
const SAMHeaderLine::SAMHeaderRecordTypes SAMHeaderLine::std_sam_header_record_types = {{"@HD", "@SQ", "@RG", "@PG", "@CO"}};

/// Default initialization.
SAMHeaderLine::SAMHeaderLine() {}

/// Initialize header line with record type, mandatory fields, optional
/// fields, and comment.
SAMHeaderLine::SAMHeaderLine(const std::string& record_type, bool parse_record_type, bool flush_ostream) : record_type{record_type}, parse_record_type{parse_record_type}, flush_ostream{flush_ostream}
{
    // Generate the line.
    line = record_type;
    // Parse record type.
    if(parse_record_type) parseRecordType();
}

/// Initialize header line with entire line string.
SAMHeaderLine::SAMHeaderLine(const std::string& line, bool parse_line, bool parse_record_type, bool flush_ostream) : line{line}, parse_record_type{parse_record_type}, flush_ostream{flush_ostream}
{
    // Parse line and assign record type.
    if(parse_line) parseLine();
    // Parse record type.
    if(parse_record_type) parseRecordType();
}

/// Initialize header line with record type, mandatory fields, optional
/// fields, and comment.
SAMHeaderLine::SAMHeaderLine(std::string&& record_type, bool parse_record_type, bool flush_ostream) : record_type{std::move(record_type)}, parse_record_type{parse_record_type}, flush_ostream{flush_ostream}
{
    // Generate the line.
    line = record_type;
    // Parse record type.
    if(parse_record_type) parseRecordType();
}

/// Initialize header line with entire line string.
SAMHeaderLine::SAMHeaderLine(std::string&& line, bool parse_line, bool parse_record_type, bool flush_ostream) : line{std::move(line)}, parse_record_type{parse_record_type}, flush_ostream{flush_ostream}
{
    // Parse line and assign record type.
    if(parse_line) parseLine();
    // Parse record type.
    if(parse_record_type) parseRecordType();
}

SAMHeaderLine::SAMHeaderLine(const SAMHeaderLine& header_line) : line{header_line.line}, record_type{header_line.record_type}, parse_line{header_line.parse_line}, parse_record_type{header_line.parse_record_type}, flush_ostream{header_line.flush_ostream} {}

SAMHeaderLine::SAMHeaderLine(SAMHeaderLine&& header_line) : line{std::move(header_line.line)}, record_type{std::move(header_line.record_type)}, parse_line{header_line.parse_line}, parse_record_type{header_line.parse_record_type}, flush_ostream{header_line.flush_ostream}
{
    header_line.reset();
}

SAMHeaderLine::~SAMHeaderLine() noexcept {}

SAMHeaderLine& SAMHeaderLine::operator=(const SAMHeaderLine& header_line)
{
    if(this != &header_line)
    {
        line = header_line.line;
        record_type = header_line.record_type;
        parse_line = header_line.parse_line;
        parse_record_type = header_line.parse_record_type;
        flush_ostream = header_line.flush_ostream;
    }
    return *this;
}

SAMHeaderLine& SAMHeaderLine::operator=(SAMHeaderLine&& header_line)
{
    if(this != &header_line)
    {
        line = std::move(header_line.line);
        record_type = std::move(header_line.record_type);
        parse_line = header_line.parse_line;
        parse_record_type = header_line.parse_record_type;
        flush_ostream = header_line.flush_ostream;
        header_line.reset();
    }
    return *this;
}

/// Clear all data members.
void SAMHeaderLine::reset()
{
    line.clear();
    record_type.clear();
    parse_line = false;
    parse_record_type = false;
    flush_ostream = false;
}

/// Parse top-level structure of header line.
void SAMHeaderLine::parseLine()
{
    std::vector<std::string> parts = utk::splitString(line, tab_sep);
    if(parts.size() > 0)
    {
        // Assign record type.
        record_type = parts.front();
    }
    else throw std::logic_error("Header line is empty!");
}

/// Parse record type of header line based on all standard record types.
void SAMHeaderLine::parseRecordType()
{
    if(record_type.length() > 0)
    {
        if(auto it_end = std::end(std_sam_header_record_types); std::find(std::begin(std_sam_header_record_types),it_end,record_type) == it_end)
        {
            std::ostringstream err_msg;
            err_msg << record_type << " is not a standard record type for header line!";
            throw std::logic_error(err_msg.str());
        }
    }
    else throw std::logic_error("Record type is empty!");
}

/// Print data field of SAM header section.
std::ostream& operator<<(std::ostream& os, const SAMHeaderLine& header_line)
{
    os << header_line.getLine();
    if(header_line.getFlushOstream()) os.flush();
    return os;
}

}
