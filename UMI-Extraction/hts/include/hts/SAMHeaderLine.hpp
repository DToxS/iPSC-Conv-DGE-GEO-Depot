//
//  SAMHeaderLine.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 3/20/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#ifndef SAMHeaderLine_hpp
#define SAMHeaderLine_hpp

#include <string>
#include <array>
#include <vector>
#include <ostream>

namespace hts
{

/// \brief The structure of a base header line of a SAM file.
/// This class defines the base structure of a line contained in the header
/// section of a SAM file, as specified by the document:
///
/// Sequence Alignment/Map Format Specification
/// The SAM/BAM Format Specification Working Group
/// June 1, 2017
class SAMHeaderLine
{
public:

    /// Data type for standard SAM record types.
    static constexpr std::size_t n_sam_header_record_types {5};
    using SAMHeaderRecordTypes = std::array<std::string, n_sam_header_record_types>;

protected:

    /// The beginning character of header line.
    static constexpr char beg_char {'@'};

    /// The separator between mandatory and optional header fields.
    static constexpr char tab_sep {'\t'};

    /// Standard record types and field tags of SAM header.
    static const SAMHeaderRecordTypes std_sam_header_record_types;

protected:

    /// Buffer for entire SAM header line.
    std::string line;

    /// Record type code of SAM header line.
    std::string record_type;

    /// Indicator for validating top-level header line structure.
    /// Note: all components will be assigned during line parsing.
    bool parse_line {false};

    /// Indicator for validating record type.
    bool parse_record_type {false};

    /// \brief Flush each written header line from output stream to disk.
    /// Note: this option needs to be switched on in a multi-threading envronment.
    bool flush_ostream {false};

protected:

    /// Clear all data members.
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
    SAMHeaderLine();

    /// \brief Initialization with header line components.
    /// Initialize header line with record type, mandatory fields, optional
    /// fields, and comment.
    SAMHeaderLine(const std::string& record_type, bool parse_record_type, bool flush_ostream);

    /// \brief Initialization with unparsed line string.
    /// Initialize header line with entire line string.
    SAMHeaderLine(const std::string& line, bool parse_line, bool parse_record_type, bool flush_ostream);

    /// \brief Initialization with header line components.
    /// Initialize header line with record type, mandatory fields, optional
    /// fields, and comment.
    SAMHeaderLine(std::string&& record_type, bool parse_record_type, bool flush_ostream);

    /// \brief Initialization with unparsed line string.
    /// Initialize header line with entire line string.
    SAMHeaderLine(std::string&& line, bool parse_line, bool parse_record_type, bool flush_ostream);

    SAMHeaderLine(const SAMHeaderLine& header_line);

    SAMHeaderLine(SAMHeaderLine&& header_line);

    virtual ~SAMHeaderLine() noexcept;

    SAMHeaderLine& operator=(const SAMHeaderLine& header_line);

    SAMHeaderLine& operator=(SAMHeaderLine&& header_line);

    /// Check if header line is empty.
    bool empty() const
    {
        return line.empty();
    }

    static char getBeginChar()
    {
        return beg_char;
    }

    static char getSeparator()
    {
        return tab_sep;
    }

    /// Get standard record types of SAM header.
    static const SAMHeaderRecordTypes& getStdSAMHeaderRecordTypes()
    {
        return std_sam_header_record_types;
    }

    /// Get the buffer for entire SAM header line.
    const std::string& getLine() const
    {
        return line;
    }

    /// Get record type code of SAM header line.
    const std::string& getRecordType() const
    {
        return record_type;
    }

    /// Get the indicator of parse line.
    bool getParseLine() const
    {
        return parse_line;
    }

    /// Get indicator for validating record type.
    bool getParseRecordType() const
    {
        return parse_record_type;
    }

    /// Get the indicator of flushing output stream.
    bool getFlushOstream() const
    {
        return flush_ostream;
    }

    /// Parse top-level structure of header line.
    virtual void parseLine();

    /// Parse record type of base header line.
    virtual void parseRecordType();
};

/// Type of SAMHeaderLine list.
using SAMHeaderLines = std::vector<SAMHeaderLine>;

/// Print a header line of SAM file.
std::ostream& operator<<(std::ostream& os, const SAMHeaderLine& header_line);

}

#endif /* SAMHeaderLine_hpp */
