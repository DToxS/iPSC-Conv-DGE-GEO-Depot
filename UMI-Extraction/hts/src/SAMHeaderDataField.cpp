//
//  SAMHeaderDataField.cpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 3/20/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#include <regex>
#include <utility>
#include <utk/StringUtils.hpp>
#include <hts/SAMHeaderDataField.hpp>

namespace hts
{

const SAMHeaderDataField::SAMHeaderDataFieldTagValueRegexes SAMHeaderDataField::std_sam_header_data_field_tag_value_regexes =
{
    {"@HD",
        {{"VN", "^[0-9]+\\.[0-9]+$"}, // Format version
        {"SO", ""}, // Sorting order of alignments
        {"GO", ""}} // Grouping of alignments
    },
    {"@SQ",
        {{"SN", "[!-)+-<>-~][!-~]*"}, // Reference sequence name
        {"LN", ""}, // Reference sequence length
        {"AH", ""}, // Indicates that this sequence is an alternate locus
        {"AN", "^[0-9A-Za-z][0-9A-Za-z\\*+\\.@ |\\-]*(,[0-9A-Za-z][0-9A-Za-z\\*+\\.@ |\\-]*)*$"}, // Alternative reference sequence names
        {"AS", ""}, // Genome assembly identifier
        {"M5", "^[\\*0-9A-F]{32}$"}, // MD5 checksum of the sequence
        {"SP", ""}, // Species
        {"UR", ""}} // URI of the sequence
    },
    {"@RG",
        {{"ID", ""}, // Read group identifier
        {"CN", ""}, // Name of sequencing center producing the read
        {"DS", ""}, // Description
        {"DT", ""}, // Date the run was produced
        {"FO", "\\*|[ACMGRSVTWYHKDBN]+"}, // Flow order
        {"KS", ""}, // The array of nucleotide bases that correspond to the key sequence of each read
        {"LB", ""}, // Library
        {"PG", ""}, // Programs used for processing the read group
        {"PI", ""}, // Predicted median insert size
        {"PL", "CAPILLARY|LS454|ILLUMINA|SOLID|HELICOS|IONTORRENT|ONT|PACBIO"}, // Platform/technology used to produce the reads
        {"PM", ""}, // Platform model
        {"PU", ""}, // Platform unit
        {"SM", ""}} // Sample
    },
    {"@PG",
        {{"ID", ""}, // Program record identifier
        {"PN", ""}, // Program name
        {"CL", ""}, // Command line
        {"PP", ""}, // Previous @PG-ID
        {"DS", ""}, // Description
        {"VN", ""}} // Program version
    }
};

/// Default initializer.
SAMHeaderDataField::SAMHeaderDataField() {}

/// Initialize optional field with tag and value.
SAMHeaderDataField::SAMHeaderDataField(const std::string& tag, const std::string& value, const std::string& record_type, bool parse_tag, bool parse_value, bool flush_ostream) : tag{tag}, value{value}, record_type{record_type}, parse_field{false}, parse_tag{parse_tag}, parse_value{parse_value}, flush_ostream{flush_ostream}
{
    // Generate field.
    field = tag + colon_sep + value;
    // Parse tag and value of data field.
    parseParts();
}

/// Initialize data field with entire field string.
SAMHeaderDataField::SAMHeaderDataField(const std::string& field, const std::string& record_type, bool parse_field, bool parse_tag, bool parse_value, bool flush_ostream) : field{field}, record_type{record_type}, parse_field{parse_field}, parse_tag{parse_tag}, parse_value{parse_value}, flush_ostream{flush_ostream}
{
    // Parse field and assign tag and value.
    if(parse_field) parseField();
    // Parse tag and value of data field.
    parseParts();
}

/// Initialize optional field with tag and value.
SAMHeaderDataField::SAMHeaderDataField(std::string&& tag, std::string&& value, std::string&& record_type, bool parse_tag, bool parse_value, bool flush_ostream) : tag{std::move(tag)}, value{std::move(value)}, record_type{std::move(record_type)}, parse_tag{parse_tag}, parse_value{parse_value}, flush_ostream{flush_ostream}
{
    // Generate field.
    field = tag + colon_sep + value;
    // Parse tag and value of data field.
    parseParts();
}

/// Initialize data field with entire field string.
SAMHeaderDataField::SAMHeaderDataField(std::string&& field, std::string&& record_type, bool parse_field, bool parse_tag, bool parse_value, bool flush_ostream) : field{std::move(field)}, record_type{std::move(record_type)}, parse_field{parse_field}, parse_tag{parse_tag}, parse_value{parse_value}, flush_ostream{flush_ostream}
{
    // Parse field and assign tag and value.
    if(parse_field) parseField();
    // Parse tag and value of data field.
    parseParts();
}

SAMHeaderDataField::SAMHeaderDataField(const SAMHeaderDataField& data_field) : field{data_field.field}, tag{data_field.tag}, value{data_field.value}, record_type{data_field.record_type}, parse_field{data_field.parse_field}, parse_tag{data_field.parse_tag}, parse_value{data_field.parse_value}, flush_ostream{data_field.flush_ostream} {}

SAMHeaderDataField::SAMHeaderDataField(SAMHeaderDataField&& data_field) : field{std::move(data_field.field)}, tag{std::move(data_field.tag)}, value{std::move(data_field.value)}, record_type{std::move(data_field.record_type)}, parse_field{data_field.parse_field}, parse_tag{data_field.parse_tag}, parse_value{data_field.parse_value}, flush_ostream{data_field.flush_ostream}
{
    data_field.reset();
}

SAMHeaderDataField::~SAMHeaderDataField() noexcept {}

SAMHeaderDataField& SAMHeaderDataField::operator=(const SAMHeaderDataField& data_field)
{
    if(this != &data_field)
    {
        field = data_field.field;
        tag = data_field.tag;
        value = data_field.value;
        record_type = data_field.record_type;
        parse_field = data_field.parse_field;
        parse_tag = data_field.parse_tag;
        parse_value = data_field.parse_value;
        flush_ostream = data_field.flush_ostream;
    }
    return *this;
}

SAMHeaderDataField& SAMHeaderDataField::operator=(SAMHeaderDataField&& data_field)
{
    if(this != &data_field)
    {
        field = std::move(data_field.field);
        tag = std::move(data_field.tag);
        value = std::move(data_field.value);
        record_type = std::move(data_field.record_type);
        parse_field = data_field.parse_field;
        parse_tag = data_field.parse_tag;
        parse_value = data_field.parse_value;
        flush_ostream = data_field.flush_ostream;
        data_field.reset();
    }
    return *this;
}

/// Clear all data members.
void SAMHeaderDataField::reset()
{
    field.clear();
    tag.clear();
    value.clear();
    record_type.clear();
    parse_field = false;
    parse_tag = false;
    parse_value = false;
    flush_ostream = false;
}

/// Parse top-level structure of data field.
void SAMHeaderDataField::parseField()
{
    std::vector<std::string> parts = utk::splitString(field, colon_sep);
    if(parts.size() == n_field_parts)
    {
        auto it = parts.cbegin();
        // Assign tag.
        tag = *(it++);
        if(tag.length() == 0)
        {
            std::ostringstream err_msg;
            err_msg << "Tag of " << field << " is empty!";
            throw std::logic_error(err_msg.str());
        }
        // Assign value.
        value = *(it++);
        if(value.length() == 0)
        {
            std::ostringstream err_msg;
            err_msg << "Value of " << field << " is empty!";
            throw std::logic_error(err_msg.str());
        }
    }
    else
    {
        std::ostringstream err_msg;
        err_msg << field << " doesn't contain " << n_field_parts << " parts!";
        throw std::logic_error(err_msg.str());
    }
}

/// Parse tag and value of data field.
void SAMHeaderDataField::parseParts()
{
    if((!parse_field) && (parse_tag || parse_value)) throw std::logic_error("parse_field must be true if either of parse_tag and parse_value is true!");
    if(parse_tag || parse_value) parseRecordType();
    if(parse_value) parseTagValue();
    else if(parse_tag) parseTag();
}

/// Parse the tag of data field.
void SAMHeaderDataField::parseTag()
{
    if(!tag.empty())
    {
        const auto& tag_value_regexes = std_sam_header_data_field_tag_value_regexes.at(record_type);
        if(auto search = tag_value_regexes.find(tag); search == tag_value_regexes.end())
        {
            std::ostringstream err_msg;
            err_msg << tag << " is not a standard tag of record type " << record_type << " of data field of SAM header line!";
            throw std::logic_error(err_msg.str());
        }
    }
    else
    {
        std::ostringstream err_msg;
        err_msg << "Tag of " << field << " is empty!";
        throw std::logic_error(err_msg.str());
    }
}

/// Parse the value of data field.
void SAMHeaderDataField::parseTagValue()
{
    if(!(value.empty() || tag.empty()))
    {
        const auto& tag_value_regexes = std_sam_header_data_field_tag_value_regexes.at(record_type);
        if(auto search = tag_value_regexes.find(tag); search != tag_value_regexes.end())
        {
            // Only parse the value for standard types.
            if(const std::string& value_regex_str = search->second; value_regex_str.length() > 0)
            {
                std::regex value_regex(value_regex_str);
                if(!std::regex_match(value, value_regex))
                {
                    std::ostringstream err_msg;
                    err_msg << value << " doesn't match with required value pattern " << value_regex_str << " of tag " << tag << " of record type " << record_type << '!';
                    throw std::logic_error(err_msg.str());
                }
            }
        }
        else
        {
            std::ostringstream err_msg;
            err_msg << tag << " is not a standard tag of record type " << record_type << " of data field of SAM header line!";
            throw std::logic_error(err_msg.str());
        }
    }
    if(tag.empty())
    {
        std::ostringstream err_msg;
        err_msg << "Tag of " << field << " is empty!";
        throw std::logic_error(err_msg.str());
    }
    if(value.empty())
    {
        std::ostringstream err_msg;
        err_msg << "Value of " << field << " is empty!";
        throw std::logic_error(err_msg.str());
    }
}

/// Parse the tag of data field.
void SAMHeaderDataField::parseRecordType()
{
    if(!record_type.empty())
    {
        if(auto search = std_sam_header_data_field_tag_value_regexes.find(record_type); search == std_sam_header_data_field_tag_value_regexes.end())
        {
            std::ostringstream err_msg;
            err_msg << record_type << " is not a standard record type of data field of SAM header line!";
            throw std::logic_error(err_msg.str());
        }
    }
    else
    {
        std::ostringstream err_msg;
        err_msg << "Record type of " << field << " is empty!";
        throw std::logic_error(err_msg.str());
    }
}

/// Print data field of SAM header section.
std::ostream& operator<<(std::ostream& os, const SAMHeaderDataField& data_field)
{
    os << data_field.getField();
    if(data_field.getFlushOstream()) os.flush();
    return os;
}

}
