//
//  SAMHeaderDataLine.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 3/23/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#ifndef SAMHeaderDataLine_hpp
#define SAMHeaderDataLine_hpp

#include <map>
#include "SAMHeaderLine.hpp"
#include "SAMHeaderDataField.hpp"

namespace hts
{

/// \brief The structure of a data header line of a SAM file.
/// This class defines the structure of a data line contained in the header
/// section of a SAM file, as specified by the document:
///
/// Sequence Alignment/Map Format Specification
/// The SAM/BAM Format Specification Working Group
/// June 1, 2017
///
/// Note1: the mandatory and optional fields are responsible for parsing
///       themselves using SAMHeaderDataField class. The parsing tasks of this
///       class only include:
///       1) Check the validity of record type according to the defined standard
///          record types.
///       2) Check if each record type includes a correct set of mandatory and
///          optional data fields according to the standard.
class SAMHeaderDataLine : public SAMHeaderLine
{
public:
    
    /// Data type of standard record types and field tags of SAM data header format.
    using SAMHeaderTypeTags = std::map<std::string, std::pair<std::vector<std::string>, std::vector<std::string>>>;

private:

    /// Standard record types and field tags of SAM data header format.
    static const SAMHeaderTypeTags std_sam_data_header_type_tags;

private:

    /// Mandatory data fields of SAM header line.
    SAMHeaderDataFields mand_fields;

    /// Optional data fields of SAM header line.
    SAMHeaderDataFields opt_fields;

    /// Indicator for validating the top structure of data fields.
    bool parse_field {false};

    /// Indicator for validating the tag of data fields.
    bool parse_tag {false};
    
    /// Indicator for validating the value of data fields.
    bool parse_value {false};

protected:

    /// Clear all data member.
    /// Note:
    /// Since this function erases the contents of all data members and reset
    /// them to initial states, it is usually called by the copy constructor and
    /// assignment operator enabled with move semantics to reset the object
    /// being copied or assigned. Therefore it's critical to NOT make this
    /// function virtual, because otherwise the same function in derived class
    /// will be actually called instead of the one in base class. Such an
    /// unexpected behavior can cause the data members in derived class to be
    /// erased before they are used in the move-enabled copy constructor and
    /// assignment operator if the copy constructor and assignment operator of
    /// the base class are called before these data members are accessed.
    void reset();

public:

    /// \brief Deault initialization.
    SAMHeaderDataLine();

    /// \brief Initialization with header line components.
    /// Initialize header line with record type, mandatory fields, and
    /// optional fields.
    SAMHeaderDataLine(const std::string& record_type_val, const SAMHeaderDataFields& mand_fields_val, const SAMHeaderDataFields& opt_fields_val, bool parse_record_type=true, bool flush_ostream=false);

    /// \brief Initialization with unparsed line string.
    /// Initialize header line with entire line string.
    /// Note: parse_record_type must be set to true if parse_line is true,
    ///       because parseLine needs to use the correctly parsed record type
    ///       to determine whether to assign each part to the mandatory or
    ///       optional group according to record type.
    SAMHeaderDataLine(const std::string& line, bool parse_line=true, bool parse_record_type=true, bool parse_field=true, bool parse_tag=false, bool parse_value=false, bool flush_ostream=false);

    /// \brief Initialization with header line components.
    /// Initialize header line with record type, mandatory fields, and
    /// optional fields.
    SAMHeaderDataLine(std::string&& record_type_val, SAMHeaderDataFields&& mand_fields_val, SAMHeaderDataFields&& opt_fields_val, bool parse_record_type=true, bool flush_ostream=false);

    /// \brief Initialization with unparsed line string.
    /// Initialize header line with entire line string.
    /// Note: parse_record_type must be set to true if parse_line is true,
    ///       because parseLine needs to use the correctly parsed record type
    ///       to determine whether to assign each part to the mandatory or
    ///       optional group according to record type.
    SAMHeaderDataLine(std::string&& line, bool parse_line=true, bool parse_record_type=true, bool parse_field=true, bool parse_tag=false, bool parse_value=false, bool flush_ostream=false);

    SAMHeaderDataLine(const SAMHeaderDataLine& header_line);

    SAMHeaderDataLine(SAMHeaderDataLine&& header_line);

    virtual ~SAMHeaderDataLine() noexcept;

    SAMHeaderDataLine& operator=(const SAMHeaderDataLine& header_line);

    SAMHeaderDataLine& operator=(SAMHeaderDataLine&& header_line);

    static const SAMHeaderTypeTags& getStdSAMDataHeaderTypeTags()
    {
        return std_sam_data_header_type_tags;
    }

    /// Get mandatory data fields of SAM header line.
    const SAMHeaderDataFields& getMandatoryFields() const
    {
        return mand_fields;
    }

    /// Get optional data fields of SAM header line.
    const SAMHeaderDataFields& getOptionalFields() const
    {
        return opt_fields;
    }

    /// Get indicator for validating the tag of data fields.
    bool getParseTag() const
    {
        return parse_tag;
    }

    /// Get indicator for validating the value of data fields.
    bool getParseValue() const
    {
        return parse_value;
    }

    /// Parse top-level structure of data header line.
    virtual void parseLine() override;

    /// Parse record type of data header line.
    virtual void parseRecordType() override;
};

/// Type of SAMHeaderDataLine list.
using SAMHeaderDataLines = std::vector<SAMHeaderDataLine>;

}

#endif /* SAMHeaderDataLine_hpp */
