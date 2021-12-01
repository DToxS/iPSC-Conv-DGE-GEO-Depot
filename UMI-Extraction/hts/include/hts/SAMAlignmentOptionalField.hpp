//
//  SAMAlignmentOptionalField.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 3/16/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#ifndef SAMAlignmentOptionalField_hpp
#define SAMAlignmentOptionalField_hpp

#include <string>
#include <vector>
#include <map>
#include <ostream>
#include <cstddef>

namespace hts
{

/// \brief The structure of an optional field of a SAM file.
/// This class defines the structure of optional fields contained in alignment
/// lines of a SAM file, as specified by the document:
///
/// Sequence Alignment/Map Optional Fields Specification
/// The SAM/BAM Format Specification Working Group
/// August 25, 2017
///
/// Note: this class is responsible for parsing its tag, type, and value.
class SAMAlignmentOptionalField
{
public:

    /// Standard tags and types of optional fields of SAM alignment format.
    using SAMAlignmentOptionalFieldTagTypes = std::map<std::string, char>;
    static const SAMAlignmentOptionalFieldTagTypes std_sam_align_opt_field_tag_types;

    /// Standard tags of optional fields of SAM alignment format.
    static const std::string std_sam_align_opt_field_tag_regex;

    /// Standard types and value regex of optional fields of SAM alignment format.
    using SAMAlignmentOptionalFieldTypeValueRegexes = std::map<char, std::string>;
    static const SAMAlignmentOptionalFieldTypeValueRegexes std_sam_align_opt_field_type_value_regexes;

private:

    /// The separator of the parts of optional field.
    static constexpr char colon_sep {':'};

    /// The number of the parts of an optional field.
    static constexpr std::size_t n_field_parts {3};

private:

    /// Buffer for entire optional field.
    std::string field;

    /// Three parts of an optional field specified by SAM standard:
    /// TAG, TYPE, and VALUE separated by colon.
    std::string tag;
    char type {'\0'};
    std::string value;

    /// Indicator for validating top-level field structure.
    /// Note: tag, type, and value will be assigned during field parsing.
    bool parse_field {false};
    /// Indicator for validating the tag.
    bool parse_tag {false};
    /// Indicator for validating the type.
    bool parse_type {false};
    /// Indicator for validating the value of standard type.
    bool parse_value {false};

    /// \brief Flush each written optional field from output stream to disk.
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
    SAMAlignmentOptionalField();

    /// \brief Initialization with optional field components.
    /// Initialize optional field with tag, type, and value.
    SAMAlignmentOptionalField(const std::string& tag, char type, const std::string& value, bool parse_tag=false, bool parse_type=false, bool parse_value=false, bool flush_ostream=false);

    /// \brief Initialization with optional field components.
    /// Initialize optional field with tag, type, and value.
    SAMAlignmentOptionalField(std::string&& tag, char type, std::string&& value, bool parse_tag=false, bool parse_type=false, bool parse_value=false, bool flush_ostream=false);

    /// \brief Initialization with unparsed field string.
    /// Initialize optional field with entire field string.
    SAMAlignmentOptionalField(const std::string& field, bool parse_field=true, bool parse_tag=false, bool parse_type=false, bool parse_value=false, bool flush_ostream=false);

    /// \brief Initialization with unparsed field string.
    /// Initialize optional field with entire field string.
    SAMAlignmentOptionalField(std::string&& field, bool parse_field=true, bool parse_tag=false, bool parse_type=false, bool parse_value=false, bool flush_ostream=false);

    SAMAlignmentOptionalField(const SAMAlignmentOptionalField& opt_field);

    SAMAlignmentOptionalField(SAMAlignmentOptionalField&& opt_field);

    virtual ~SAMAlignmentOptionalField() noexcept;

    SAMAlignmentOptionalField& operator=(const SAMAlignmentOptionalField& opt_field);

    SAMAlignmentOptionalField& operator=(SAMAlignmentOptionalField&& opt_field);

    /// Check if optional field is empty.
    bool empty() const
    {
        return field.empty();
    }

    static const SAMAlignmentOptionalFieldTagTypes& getStdSAMAlignmentOptionalFieldTagTypes()
    {
        return std_sam_align_opt_field_tag_types;
    }

    static const std::string& getStdSAMAlignmentOptionalFieldTagRegex()
    {
        return std_sam_align_opt_field_tag_regex;
    }

    static const SAMAlignmentOptionalFieldTypeValueRegexes& getStdSAMAlignmentOptionalFieldTypeValueRegexes()
    {
        return std_sam_align_opt_field_type_value_regexes;
    }

    static std::size_t getNumberOfFieldParts()
    {
        return n_field_parts;
    }

    static char getSeparator()
    {
        return colon_sep;
    }

    /// Get entire optional field.
    const std::string& getField() const
    {
        return field;
    }

    /// Get the tag of optional field.
    const std::string& getTag() const
    {
        return tag;
    }

    /// Get the type of optional field.
    char getType() const
    {
        return type;
    }

    /// Get the value of optional field.
    const std::string& getValue() const
    {
        return value;
    }

    /// Get the indicator of flushing output stream.
    bool getFlushOstream() const
    {
        return flush_ostream;
    }

    /// Parse top-level structure of optional field.
    void parseField();

    /// Parse tag, type, and value of optional field.
    void parseParts();

    /// Parse the tag of optional field.
    void parseTag();

    /// Parse the tag and type of optional field.
    void parseTagType();

    /// Parse the value of standard type for optional field.
    void parseValue();
};

/// Type of the list of composing parts for optional fields.
using SAMAlignmentOptionalFieldParts = std::vector<std::string>;

/// Print optional field of SAM alignment section.
std::ostream& operator<<(std::ostream& os, const SAMAlignmentOptionalField& opt_field);

}

#endif /* SAMAlignmentOptionalField_hpp */
