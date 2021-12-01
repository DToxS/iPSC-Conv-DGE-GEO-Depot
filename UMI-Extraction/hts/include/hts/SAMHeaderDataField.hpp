//
//  SAMHeaderDataField.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 3/20/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#ifndef SAMHeaderDataField_hpp
#define SAMHeaderDataField_hpp

#include <map>
#include <string>
#include <vector>
#include <ostream>

namespace hts
{

/// \brief Data fields of header section of SAM file.
/// This class defines the structure of a data field contained in the header
/// section of a SAM file, as specified by the document:
///
/// Sequence Alignment/Map Format Specification
/// The SAM/BAM Format Specification Working Group
/// June 1, 2017
///
/// Note: this class is responsible for parsing its tag and value.
class SAMHeaderDataField
{
public:

    /// Data type of standard tags and value regex of data fields of SAM header.
//    using SAMHeaderDataFieldTagValueRegexes = std::map<std::string, std::string>;
    using SAMHeaderDataFieldTagValueRegexes = std::map<std::string, std::map<std::string, std::string>>;

private:

    /// The separator of the parts of data field.
    static constexpr char colon_sep {':'};

    /// The number of the parts of an data field.
    static constexpr std::size_t n_field_parts {2};

    /// Standard tags and value regex of data fields of SAM header.
    static const SAMHeaderDataFieldTagValueRegexes std_sam_header_data_field_tag_value_regexes;

private:

    /// Buffer for entire data field.
    std::string field;

    /// Two parts of a data field specified by SAM standard:
    /// TAG and VALUE separated by colon.
    std::string tag;
    std::string value;

    /// Record type that this data field belongs to.
    std::string record_type;

    /// Indicator for validating top-level field structure.
    /// Note: tag and value will be assigned during field parsing.
    bool parse_field {false};
    /// Indicator for validating the tag.
    bool parse_tag {false};
    /// Indicator for validating the value of standard tag.
    bool parse_value {false};

    /// \brief Flush each written data field from output stream to disk.
    /// Note: this option needs to be switched on in a multi-threading envronment.
    bool flush_ostream {false};

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

    /// Default initializer.
    SAMHeaderDataField();

    /// \brief Initialization with data field components.
    /// Initialize optional field with tag and value.
    SAMHeaderDataField(const std::string& tag, const std::string& value, const std::string& record_type, bool parse_tag=false, bool parse_value=false, bool flush_ostream=false);

    /// \brief Initialization with unparsed field string.
    /// Initialize data field with entire field string.
    SAMHeaderDataField(const std::string& field, const std::string& record_type, bool parse_field=true, bool parse_tag=false, bool parse_value=false, bool flush_ostream=false);

    /// \brief Initialization with data field components.
    /// Initialize optional field with tag and value.
    SAMHeaderDataField(std::string&& tag, std::string&& value, std::string&& record_type, bool parse_tag=false, bool parse_value=false, bool flush_ostream=false);

    /// \brief Initialization with unparsed field string.
    /// Initialize data field with entire field string.
    SAMHeaderDataField(std::string&& field, std::string&& record_type, bool parse_field=true, bool parse_tag=false, bool parse_value=false, bool flush_ostream=false);

    SAMHeaderDataField(const SAMHeaderDataField& data_field);

    SAMHeaderDataField(SAMHeaderDataField&& data_field);

    virtual ~SAMHeaderDataField() noexcept;

    SAMHeaderDataField& operator=(const SAMHeaderDataField& data_field);

    SAMHeaderDataField& operator=(SAMHeaderDataField&& data_field);

    static const SAMHeaderDataFieldTagValueRegexes& getStdSAMHeaderDataFieldTagValueRegexes()
    {
        return std_sam_header_data_field_tag_value_regexes;
    }

    /// Check if data field is empty.
    bool empty() const
    {
        return field.empty();
    }

    static char getSeparator()
    {
        return colon_sep;
    }

    std::size_t getNumberOfFieldParts() const
    {
        return n_field_parts;
    }

    /// Get entire data field.
    const std::string& getField() const
    {
        return field;
    }

    /// Get the tag of data field.
    const std::string& getTag() const
    {
        return tag;
    }

    /// Get the value of data field.
    const std::string& getValue() const
    {
        return value;
    }

    /// Get the record type of data field.
    const std::string& getRecordType() const
    {
        return record_type;
    }

    /// Get the indicator of flushing output stream.
    bool getFlushOstream() const
    {
        return flush_ostream;
    }

    /// Parse top-level structure of data field.
    void parseField();

    /// Parse tag and value of data field.
    void parseParts();

    /// Parse the tag of data field.
    void parseTag();

    /// Parse the value of data field.
    void parseTagValue();

    /// Parse the record type of data field.
    void parseRecordType();
};

/// Type of SAMHeaderDataField list.
using SAMHeaderDataFields = std::vector<SAMHeaderDataField>;

/// Print data field of SAM data section.
std::ostream& operator<<(std::ostream& os, const SAMHeaderDataField& data_field);

}

#endif /* SAMHeaderDataField_hpp */
