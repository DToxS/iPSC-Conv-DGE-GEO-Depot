//
//  SAMHeaderCommentLine.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 3/23/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#ifndef SAMHeaderCommentLine_hpp
#define SAMHeaderCommentLine_hpp

#include "SAMHeaderLine.hpp"

namespace hts
{

/// \brief The structure of a comment header line of a SAM file.
/// This class defines the structure of a comment line contained in the header
/// section of a SAM file, as specified by the document:
///
/// Sequence Alignment/Map Format Specification
/// The SAM/BAM Format Specification Working Group
/// June 1, 2017
class SAMHeaderCommentLine : public SAMHeaderLine
{
private:

    /// Standard record type of comment header line of SAM file.
    static const std::string std_sam_comment_header_record_type;

    /// Comments of SAM header line.
    std::string comment_field;

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
    SAMHeaderCommentLine();

    /// \brief Initialization with header line components.
    /// Initialize header line with comment.
    SAMHeaderCommentLine(const std::string& record_type_val, const std::string& comment_field_val, bool parse_record_type=true, bool flush_ostream=false);

    /// \brief Initialization with unparsed line string.
    /// Initialize header line with entire line string.
    /// Note: parse_record_type must be set to true if parse_line is true,
    ///       because parseLine assumes the line being parsed is a type of
    ///       comment header line, which is verified by parseRecordType.
    SAMHeaderCommentLine(const std::string& line, bool parse_line=true, bool parse_record_type=true, bool flush_ostream=false);

    /// \brief Initialization with header line components.
    /// Initialize header line with comment.
    SAMHeaderCommentLine(std::string&& record_type_val, std::string&& comment_field_val, bool parse_record_type=true, bool flush_ostream=false);

    /// \brief Initialization with unparsed line string.
    /// Initialize header line with entire line string.
    /// Note: parse_record_type must be set to true if parse_line is true,
    ///       because parseLine assumes the line being parsed is a type of
    ///       comment header line, which is verified by parseRecordType.
    SAMHeaderCommentLine(std::string&& line, bool parse_line=true, bool parse_record_type=true, bool flush_ostream=false);

    SAMHeaderCommentLine(const SAMHeaderCommentLine& header_line);

    SAMHeaderCommentLine(SAMHeaderCommentLine&& header_line);

    virtual ~SAMHeaderCommentLine() noexcept;

    SAMHeaderCommentLine& operator=(const SAMHeaderCommentLine& header_line);

    SAMHeaderCommentLine& operator=(SAMHeaderCommentLine&& header_line);

    /// Get standard record type of comment header line of SAM file.
    static const std::string& getStdSAMCommentHeaderRecordType()
    {
        return std_sam_comment_header_record_type;
    }

    /// Get comments of SAM header line.
    const std::string& getCommentField() const
    {
        return comment_field;
    }

    /// Parse top-level structure of comment header line.
    virtual void parseLine() override;

    /// Parse record type of comment header line.
    virtual void parseRecordType() override;
};

/// Type of SAMHeaderCommentLine list.
using SAMHeaderCommentLines = std::vector<SAMHeaderCommentLine>;

}

#endif /* SAMHeaderCommentLine_hpp */
