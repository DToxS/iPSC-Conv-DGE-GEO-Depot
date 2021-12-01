//
//  SAMHeaderCommentLine.cpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 3/23/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#include <utility>
#include <stdexcept>
#include <utk/StringUtils.hpp>
#include <hts/SAMHeaderCommentLine.hpp>

namespace hts
{

/// Initialization of standard record type of comment header line of SAM file.
const std::string SAMHeaderCommentLine::std_sam_comment_header_record_type = {"@CO"};

/// Deault initialization.
SAMHeaderCommentLine::SAMHeaderCommentLine() : SAMHeaderLine() {}

/// Initialize header line with record type, mandatory fields, optional
/// fields, and comment.
SAMHeaderCommentLine::SAMHeaderCommentLine(const std::string& record_type_val, const std::string& comment_field_val, bool parse_record_type, bool flush_ostream) : SAMHeaderLine(record_type_val, parse_record_type, flush_ostream), comment_field{comment_field_val}
{
    // Generate the line.
    line += tab_sep + comment_field;
    // Parse record type.
    if(parse_record_type) parseRecordType();
}

/// Initialize header line with entire line string.
SAMHeaderCommentLine::SAMHeaderCommentLine(const std::string& line, bool parse_line, bool parse_record_type, bool flush_ostream) : SAMHeaderLine(line, parse_line, parse_record_type, flush_ostream)
{
    // Check the match between parse_line and parse_record_type.
    if(parse_line && !parse_record_type) throw std::logic_error("parse_record_type must be true if parse_line is true!");
    // Parse line and assign comment.
    if(parse_line) parseLine();
    // Parse record type.
    if(parse_record_type) parseRecordType();
}

/// Initialize header line with record type, mandatory fields, optional
/// fields, and comment.
SAMHeaderCommentLine::SAMHeaderCommentLine(std::string&& record_type_val, std::string&& comment_field_val, bool parse_record_type, bool flush_ostream) : SAMHeaderLine(std::move(record_type_val), parse_record_type, flush_ostream), comment_field{std::move(comment_field_val)}
{
    // Generate the line.
    line += tab_sep + comment_field;
    // Parse record type.
    if(parse_record_type) parseRecordType();
}

/// Initialize header line with entire line string.
SAMHeaderCommentLine::SAMHeaderCommentLine(std::string&& line, bool parse_line, bool parse_record_type, bool flush_ostream) : SAMHeaderLine(std::move(line), parse_line, parse_record_type, flush_ostream)
{
    // Check the match between parse_line and parse_record_type.
    if(parse_line && !parse_record_type) throw std::logic_error("parse_record_type must be true if parse_line is true!");
    // Parse line and assign comment.
    if(parse_line) parseLine();
    // Parse record type.
    if(parse_record_type) parseRecordType();
}

SAMHeaderCommentLine::SAMHeaderCommentLine(const SAMHeaderCommentLine& header_line) : SAMHeaderLine(header_line), comment_field{header_line.comment_field} {}

SAMHeaderCommentLine::SAMHeaderCommentLine(SAMHeaderCommentLine&& header_line) : SAMHeaderLine(std::move(header_line)), comment_field{std::move(header_line.comment_field)}
{
    header_line.reset();
}

SAMHeaderCommentLine::~SAMHeaderCommentLine() noexcept {}

SAMHeaderCommentLine& SAMHeaderCommentLine::operator=(const SAMHeaderCommentLine& header_line)
{
    if(this != &header_line)
    {
        SAMHeaderLine::operator=(header_line);
        comment_field = header_line.comment_field;
    }
    return *this;
}

SAMHeaderCommentLine& SAMHeaderCommentLine::operator=(SAMHeaderCommentLine&& header_line)
{
    if(this != &header_line)
    {
        SAMHeaderLine::operator=(std::move(header_line));
        comment_field = std::move(header_line.comment_field);
        header_line.reset();
    }
    return *this;
}

/// Clear all data members.
void SAMHeaderCommentLine::reset()
{
    comment_field.clear();
    SAMHeaderLine::reset();
}

/// Parse top-level structure of header line.
/// Note: This function assumes the line being parsed is a type of comment header
///       line, so that the function just concatenates all the rest parts to the
///       comment variable.
void SAMHeaderCommentLine::parseLine()
{
    std::vector<std::string> parts = utk::splitString(line, tab_sep);
    if(parts.size() > 0)
    {
        for(auto it = parts.cbegin(); it != parts.cend(); ++it)
        {
            if(it != parts.cbegin())
            {
                // Add the rest parts to comment variable.
                if(comment_field.length() > 0) comment_field += tab_sep + *it;
                else comment_field += *it;
            }
        }
    }
    else throw std::logic_error("Header line is empty!");
}

/// Parse record type of comment header line based on only standard comment
/// record type.
void SAMHeaderCommentLine::parseRecordType()
{
    if(record_type.length() > 0)
    {
        if(record_type != std_sam_comment_header_record_type)
        {
            std::ostringstream err_msg;
            err_msg << record_type << " is not a standard record type for comment header line!";
            throw std::logic_error(err_msg.str());
        }
    }
    else throw std::logic_error("Record type is empty!");
}

}

