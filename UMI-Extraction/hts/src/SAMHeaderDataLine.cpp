//
//  SAMHeaderDataLine.cpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 3/23/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#include <set>
#include <regex>
#include <algorithm>
#include <utility>
#include <sstream>
#include <stdexcept>
#include <utk/StringUtils.hpp>
#include <hts/SAMHeaderDataLine.hpp>

namespace hts
{

/// Standard record types and field tags of SAM data header format.
const SAMHeaderDataLine::SAMHeaderTypeTags SAMHeaderDataLine::std_sam_data_header_type_tags =
{
    {"@HD", {{"VN"}, {"SO","GO"}}},
    {"@SQ", {{"SN","LN"}, {"AH","AN","AS","M5","SP","UR"}}},
    {"@RG", {{"ID"}, {"CN","DS","DT","FO","KS","LB","PG","PI","PL","PM","PU","SM"}}},
    {"@PG", {{"ID"}, {"PN","CL","PP","DS","VN"}}}
};

/// Deault initialization.
SAMHeaderDataLine::SAMHeaderDataLine() : SAMHeaderLine() {}

/// Initialize header line with record type, mandatory fields, and
/// optional fields.
SAMHeaderDataLine::SAMHeaderDataLine(const std::string& record_type_val, const SAMHeaderDataFields& mand_fields_val, const SAMHeaderDataFields& opt_fields_val, bool parse_record_type, bool flush_ostream) : SAMHeaderLine(record_type_val, parse_record_type, flush_ostream), mand_fields{mand_fields_val}, opt_fields{opt_fields_val}
{
    // Generate the line.
    for(const auto& mand_field : mand_fields) line += tab_sep + mand_field.getField();
    for(const auto& opt_field : opt_fields) line += tab_sep + opt_field.getField();
    // Parse record type.
    if(parse_record_type) parseRecordType();
}

/// Initialize header line with entire line string.
SAMHeaderDataLine::SAMHeaderDataLine(const std::string& line, bool parse_line, bool parse_record_type, bool parse_field, bool parse_tag, bool parse_value, bool flush_ostream) : SAMHeaderLine(line, parse_line, parse_record_type, flush_ostream), parse_field{parse_field}, parse_tag{parse_tag}, parse_value{parse_value}
{
    // Check the match between parse_line and parse_record_type.
    if(parse_line && !parse_record_type) throw std::logic_error("parse_record_type must be true if parse_line is true!");
    // Parse record type.
    if(parse_record_type) parseRecordType();
    // Parse line and assign record type, mandatory fields, and optional fields.
    if(parse_line) parseLine();
}

/// Initialize header line with record type, mandatory fields, and
/// optional fields.
SAMHeaderDataLine::SAMHeaderDataLine(std::string&& record_type_val, SAMHeaderDataFields&& mand_fields_val, SAMHeaderDataFields&& opt_fields_val, bool parse_record_type, bool flush_ostream) : SAMHeaderLine(std::move(record_type_val), parse_record_type, flush_ostream), mand_fields{std::move(mand_fields_val)}, opt_fields{std::move(opt_fields_val)}
{
    // Generate the line.
    for(const auto& mand_field : mand_fields) line += tab_sep + mand_field.getField();
    for(const auto& opt_field : opt_fields) line += tab_sep + opt_field.getField();
    // Parse record type.
    if(parse_record_type) parseRecordType();
}

/// Initialize header line with entire line string.
SAMHeaderDataLine::SAMHeaderDataLine(std::string&& line, bool parse_line, bool parse_record_type, bool parse_field, bool parse_tag, bool parse_value, bool flush_ostream) : SAMHeaderLine(std::move(line), parse_line, parse_record_type, flush_ostream), parse_field{parse_field}, parse_tag{parse_tag}, parse_value{parse_value}
{
    // Check the match between parse_line and parse_record_type.
    if(parse_line && !parse_record_type) throw std::logic_error("parse_record_type must be true if parse_line is true!");
    // Parse record type.
    if(parse_record_type) parseRecordType();
    // Parse line and assign record type, mandatory fields, and optional fields.
    if(parse_line) parseLine();
}

SAMHeaderDataLine::SAMHeaderDataLine(const SAMHeaderDataLine& header_line) : SAMHeaderLine(header_line), mand_fields{header_line.mand_fields}, opt_fields{header_line.opt_fields}, parse_field{header_line.parse_field}, parse_tag{header_line.parse_tag}, parse_value{header_line.parse_value} {}

SAMHeaderDataLine::SAMHeaderDataLine(SAMHeaderDataLine&& header_line) : SAMHeaderLine(std::move(header_line)), mand_fields{std::move(header_line.mand_fields)}, opt_fields{std::move(header_line.opt_fields)}, parse_field{header_line.parse_field}, parse_tag{header_line.parse_tag}, parse_value{header_line.parse_value}
{
    header_line.reset();
}

SAMHeaderDataLine::~SAMHeaderDataLine() noexcept {}

SAMHeaderDataLine& SAMHeaderDataLine::operator=(const SAMHeaderDataLine& header_line)
{
    if(this != &header_line)
    {
        SAMHeaderLine::operator=(header_line);
        mand_fields = header_line.mand_fields;
        opt_fields = header_line.opt_fields;
        parse_field = header_line.parse_field;
        parse_tag = header_line.parse_tag;
        parse_value = header_line.parse_value;
    }
    return *this;
}

SAMHeaderDataLine& SAMHeaderDataLine::operator=(SAMHeaderDataLine&& header_line)
{
    if(this != &header_line)
    {
        SAMHeaderLine::operator=(std::move(header_line));
        mand_fields = std::move(header_line.mand_fields);
        opt_fields = std::move(header_line.opt_fields);
        parse_field = header_line.parse_field;
        parse_tag = header_line.parse_tag;
        parse_value = header_line.parse_value;
        header_line.reset();
    }
    return *this;
}

/// Clear all data members.
void SAMHeaderDataLine::reset()
{
    mand_fields.clear();
    opt_fields.clear();
    parse_field = false;
    parse_tag = false;
    parse_value = false;
}

/// Parse top-level structure of header line.
void SAMHeaderDataLine::parseLine()
{
    std::vector<std::string> parts = utk::splitString(line, tab_sep);
    if(parts.size() > 0)
    {
        // Obtain the lists of mandatory and optional data fields of
        // record type.
        const auto& std_mand_opt_fields = std_sam_data_header_type_tags.at(record_type);
        const auto& std_mand_fields = std_mand_opt_fields.first;
        const auto& std_opt_fields = std_mand_opt_fields.second;
        // All the tags of mandatory data fields found.
        std::vector<std::string> mand_data_field_tags;
        for(auto it = parts.cbegin(); it != parts.cend(); ++it)
        {
            if(it != parts.cbegin())
            {
                // Non-comment header line.
                // Parse the rest elements as data field and compare it with
                // standard mandatory and optional data fields, and then assign
                // them to corresponding data field list.
                // Create a data field with top-level structure parsed at least.
                SAMHeaderDataField data_field(*it, record_type, parse_field, parse_tag, parse_value, flush_ostream);
                const std::string& data_field_tag = data_field.getTag();
                // Look up the tag of data field in the list of mandatory data
                // fields.
                bool found_mand_field = std::find(std_mand_fields.cbegin(), std_mand_fields.cend(), data_field_tag) != std_mand_fields.cend();
                // Look up the tag of data field in the list of optional data
                // fields.
                bool found_opt_field = false;
                if(found_mand_field)
                {
                    mand_data_field_tags.push_back(data_field_tag);
                    mand_fields.push_back(std::move(data_field));
                }
                else
                {
                    found_opt_field = std::find(std_opt_fields.cbegin(), std_opt_fields.cend(), data_field_tag) != std_opt_fields.cend();
                    if(found_opt_field) opt_fields.push_back(std::move(data_field));
                }
                if(!(found_mand_field || found_opt_field))
                {
                    std::ostringstream err_msg;
                    err_msg << data_field << " is not a standard data field of " << record_type << " type of SAM header line!";
                    throw std::logic_error(err_msg.str());
                }
            }
        }
        // Check if mandatory fields satisfy the standdard.
        std::set<std::string> mand_data_field_tags_set(mand_data_field_tags.begin(), mand_data_field_tags.end());
        std::set<std::string> std_mand_fields_set(std_mand_fields.begin(), std_mand_fields.end());
        if(mand_data_field_tags_set != std_mand_fields_set)
        {
            std::ostringstream err_msg;
            err_msg << "The mandatory data fields of " << record_type << " doesn't match with the standard!";
            throw std::logic_error(err_msg.str());
        }
    }
    else throw std::logic_error("Header line is empty!");
}

/// Parse record type of data header line.
void SAMHeaderDataLine::parseRecordType()
{
    if(record_type.length() > 0)
    {
        if(auto search = std_sam_data_header_type_tags.find(record_type); search == std_sam_data_header_type_tags.end())
        {
            std::ostringstream err_msg;
            err_msg << record_type << " is not a standard record type for data header line!";
            throw std::logic_error(err_msg.str());
        }
    }
    else throw std::logic_error("Record type is empty!");
}

}
