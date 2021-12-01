//
//  SAMFileReader.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 3/8/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#ifndef SAMFileReader_hpp
#define SAMFileReader_hpp

#include <string>
#include <stdexcept>
#include <utk/LineReader.hpp>
#include <utk/StringUtils.hpp>
#include "SAMHeaderDataLine.hpp"
#include "SAMHeaderCommentLine.hpp"
#include "SAMAlignmentLine.hpp"

namespace hts
{

enum class SAMLineType { HeaderDataLine, HeaderCommentLine, AlignmentLine, AllLine };

/// \brief A reader to retrieve header and alignment lines from SAM file
/// This class reads in each line in header section and alignment section from
/// a SAM file, and creates and returns three types of data objects:
///
/// - SAMHeaderDataLine: a header line for data fields
/// - SAMHeaderCommentLine: a header line for comments
/// - SAMAlignmentLine: an alignment line for FASTQ sequence
template<typename SAMAlignmentLineType>
class SAMFileReader : public utk::LineReader
{
private:

    /// Indicator for parsing header line.
    bool parse_header_line {false};

    /// Indicator for parsing the top structure of each field of header line.
    bool parse_header_fields {false};

    /// Indicator for parsing the tag and value attributes of each field of
    /// header line.
    bool parse_header_fields_attribs {false};

    /// Indicator for parsing the top structure of alignment line.
    bool parse_align_line {false};

    /// Indicator for parsing all mandatory fields of alignment line according
    /// to the SAM standard.
    bool parse_mand_align_fields {false};

    /// Indicator for parsing the top structure of each optional field of
    /// alignment line.
    bool parse_opt_align_fields {false};

    /// Indicator for parsing the tag, type, and value attributes of each
    /// optional field of alignment line.
    bool parse_opt_align_fields_attribs {false};

    /// \brief The tags of preferred optional fields to be parsed.
    /// If not empty, only these preferred optional fields will be parsed while
    /// other fileds will be skipped.
    SAMAlignmentOptionalFieldParts pref_opt_fields_tags;

    /// \brief Flush each written line from output stream to disk.
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
    void reset()
    {
        parse_header_line = false;
        parse_header_fields = false;
        parse_header_fields_attribs = false;
        parse_align_line = false;
        parse_mand_align_fields = false;
        parse_opt_align_fields = false;
        parse_opt_align_fields_attribs = false;
        pref_opt_fields_tags.clear();
        flush_ostream = false;
    }

public:

    SAMFileReader() = delete;

    /// \brief Initialize SAM file reader with various control parameters.
    /// \param[in]  parse_header_line               Parse top-level structure of header lines to extract all header fields (Default: true)
    /// \param[in]  parse_header_fields             Parse top-level structure of header data fields to extract tag:value info (Default: true)
    /// \param[in]  parse_header_fields_attribs     Parse the standard conformance of tag and value of header data fields (Default: false)
    /// \param[in]  parse_align_line                Parse top-level structure of alignment lines to extract all alignment fields (Default: true)
    /// \param[in]  parse_mand_align_fields         Parse the standard conformance of mandatory alignment fields (Default: false)
    /// \param[in]  parse_opt_align_fields Parse    top-level structure of optional alignment fields to extract tag:type:value info (Default: true)
    /// \param[in]  parse_opt_align_fields_attribs  Parse the standard conformance of tag, type, and value of optionl alignment fields (Default: false)
    explicit SAMFileReader(const std::string& file_name, bool parse_header_line=true, bool parse_header_fields=true, bool parse_header_fields_attribs=false, bool parse_align_line=true, bool parse_mand_align_fields=false, bool parse_opt_align_fields=true, bool parse_opt_align_fields_attribs=false, const SAMAlignmentOptionalFieldParts& pref_opt_fields_tags=SAMAlignmentOptionalFieldParts(), bool flush_ostream=false, const std::string& line_delim_type="unix") : utk::LineReader(file_name, line_delim_type), parse_header_line{parse_header_line}, parse_header_fields{parse_header_fields}, parse_header_fields_attribs{parse_header_fields_attribs}, parse_align_line{parse_align_line}, parse_mand_align_fields{parse_mand_align_fields}, parse_opt_align_fields{parse_opt_align_fields}, parse_opt_align_fields_attribs{parse_opt_align_fields_attribs}, pref_opt_fields_tags{pref_opt_fields_tags}, flush_ostream{flush_ostream} {}

    /// Forbid copy construction behavior.
    SAMFileReader(const SAMFileReader& sam_file) = delete;

    /// Allow move construction behavior.
    SAMFileReader(SAMFileReader&& sam_file) : utk::LineReader(std::move(sam_file)), parse_header_line{sam_file.parse_header_line}, parse_header_fields{sam_file.parse_header_fields}, parse_header_fields_attribs{sam_file.parse_header_fields_attribs}, parse_align_line{sam_file.parse_align_line}, parse_mand_align_fields{sam_file.parse_mand_align_fields}, parse_opt_align_fields{sam_file.parse_opt_align_fields}, parse_opt_align_fields_attribs{sam_file.parse_opt_align_fields_attribs}, pref_opt_fields_tags{std::move(sam_file.pref_opt_fields_tags)}, flush_ostream{sam_file.flush_ostream}
    {
        sam_file.reset();
    }

    virtual ~SAMFileReader() noexcept {}

    /// Forbid copy assignment behavior.
    SAMFileReader& operator=(const SAMFileReader& sam_file) = delete;

    /// Allow copy assignment behavior.
    SAMFileReader& operator=(SAMFileReader&& sam_file)
    {
        if(this != &sam_file)
        {
            utk::LineReader::operator=(std::move(sam_file));
            parse_header_line = sam_file.parse_header_line;
            parse_header_fields = sam_file.parse_header_fields;
            parse_header_fields_attribs = sam_file.parse_header_fields_attribs;
            parse_align_line = sam_file.parse_align_line;
            parse_mand_align_fields = sam_file.parse_mand_align_fields;
            parse_opt_align_fields = sam_file.parse_opt_align_fields;
            parse_opt_align_fields_attribs = sam_file.parse_opt_align_fields_attribs;
            pref_opt_fields_tags = std::move(sam_file.pref_opt_fields_tags);
            flush_ostream = sam_file.flush_ostream;
            sam_file.reset();
        }
        return *this;
    }

    /// Read a header data line of a SAM file.
    /// \tparam      detect     An indicator for detecting the type of line.
    /// \param[out]  data_line  The created SAMHeaderDataLine object.
    /// \param[out]  read_line  The creation status.
    /// \return      The status of reading the line from a SAM file.
    /// \note        Set detect to true when the line format is unknown, otherwise
    ///              set it to false to avoid format dection for efficiency.
    template <bool detect=true>
    bool readHeaderDataLine(SAMHeaderDataLine& data_line, bool& read_line)
    {
        // Initialize the status of object creation to false.
        read_line = false;
        // Read in a line from the SAM file.
        std::string line;
        bool status = readLine(line);
        // Create a SAMHeaderDataLine object.
        if(status) read_line = readHeaderDataLine<detect>(line, data_line);
        // Return the status of line reading operation.
        return status;
    }

    /// Create an object of header data line from a character string.
    /// \tparam      detect     An indicator for detecting the type of line.
    /// \param[in]   line       A line of characters read from a SAM file.
    /// \param[out]  data_line  The created SAMHeaderDataLine object.
    /// \return      The status of object creation.
    /// \note        Either case indicated by detect is compiled while the other
    ///              is not.
    template <bool detect=true>
    bool readHeaderDataLine(const std::string& line, SAMHeaderDataLine& data_line)
    {
        // Initialize the status of object creation to false.
        bool status {false};
        if constexpr (detect)
        {
            // Detection of line type is enabled.
            // A data line can be created successfully only if:
            // 1) The line begins with SAMHeaderLine::getBeginChar() but not
            //    SAMHeaderCommentLine::getStdSAMCommentHeaderRecordType().
            // 2) The line is parsed without error.
            if(const std::string& comment_record_type = SAMHeaderCommentLine::getStdSAMCommentHeaderRecordType(); (line.front() == SAMHeaderLine::getBeginChar()) && (line.substr(0,comment_record_type.length()) != comment_record_type))
            {
                data_line = SAMHeaderDataLine(line, parse_header_line, parse_header_line, parse_header_fields, parse_header_fields_attribs, parse_header_fields_attribs, flush_ostream);
                status = true;
            }
        }
        else
        {
            // Detection of line type is disabled.
            // A data line can be created successfully if:
            // 1) The line is parsed without error.
            data_line = SAMHeaderDataLine(line, parse_header_line, parse_header_line, parse_header_fields, parse_header_fields_attribs, parse_header_fields_attribs, flush_ostream);
            status = true;
        }
        // Return the status of object creation.
        return status;
    }

    /// Read a header comment line of a SAM file.
    /// \tparam      detect        An indicator for detecting the type of line.
    /// \param[out]  comment_line  The created SAMHeaderCommentLine object.
    /// \param[out]  read_line     The creation status.
    /// \return      The status of reading the line from a SAM file.
    /// \note        Set detect to true when the line format is unknown, otherwise
    ///              set it to false to avoid format dection for efficiency.
    template <bool detect=true>
    bool readHeaderCommentLine(SAMHeaderCommentLine& comment_line, bool& read_line)
    {
        // Initialize the status of object creation to false.
        read_line = false;
        // Read in a line from the SAM file.
        std::string line;
        bool status = readLine(line);
        // Create a SAMHeaderCommentLine object.
        if(status) read_line = readHeaderCommentLine<detect>(line, comment_line);
        // Return the status of line reading operation.
        return status;
    }

    /// Create an object of header comment line from a character string.
    /// \tparam      detect          An indicator for detecting the type of line.
    /// \param[in]   line            A line of characters read from a SAM file.
    /// \param[out]  comment_line  The created SAMHeaderCommentLine object.
    /// \return      The status of object creation.
    /// \note        Either case indicated by detect is compiled while the other
    ///              is not.
    template <bool detect=true>
    bool readHeaderCommentLine(const std::string& line, SAMHeaderCommentLine& comment_line)
    {
        // Initialize the status of object creation to false.
        bool status {false};
        if constexpr (detect)
        {
            // Detection of line type is enabled.
            // A comment line can be created successfully only if:
            // 1) The line begins with SAMHeaderCommentLine::getStdSAMCommentHeaderRecordType().
            // 2) The line is parsed without error.
            if(const std::string& comment_record_type = SAMHeaderCommentLine::getStdSAMCommentHeaderRecordType(); (line.substr(0,comment_record_type.length()) == comment_record_type))
            {
                comment_line = SAMHeaderCommentLine(line, parse_header_line, parse_header_line, flush_ostream);
                status = true;
            }
        }
        else
        {
            // Detection of line type is disabled.
            // A comment line can be created successfully if:
            // 1) The line is parsed without error.
            comment_line = SAMHeaderCommentLine(line, parse_header_line, parse_header_line, flush_ostream);
            status = true;
        }
        // Return the status of object creation.
        return status;
    }

    /// Read an alignment line of a SAM file.
    /// \tparam      detect          An indicator for detecting the type of line.
    /// \param[out]  alignment_line  The created SAMAlignmentLineType object.
    /// \param[out]  read_line       The creation status.
    /// \return      The status of reading the line from a SAM file.
    /// \note        Set detect to true when the line format is unknown, otherwise
    ///              set it to false to avoid format dection for efficiency.
    template <bool detect=true>
    bool readAlignmentLine(SAMAlignmentLineType& alignment_line, bool& read_line)
    {
        // Initialize the status of object creation to false.
        read_line = false;
        // Read in a line from the SAM file.
        std::string line;
        bool status = readLine(line);
        // Create a SAMAlignmentLineType object.
        if(status) read_line = readAlignmentLine<detect>(line, alignment_line);
        // Return the status of line reading operation.
        return status;
    }

    /// Create an object of alignment line from a character string.
    /// \tparam      detect          An indicator for detecting the type of line.
    /// \param[in]   line            A line of characters read from a SAM file.
    /// \param[out]  alignment_line  The created SAMAlignmentLineType object.
    /// \return      The status of object creation.
    /// \note        Either case indicated by detect is compiled while the other
    ///              is not.
    template <bool detect=true>
    bool readAlignmentLine(const std::string& line, SAMAlignmentLineType& alignment_line)
    {
        // Initialize the status of object creation to false.
        bool status {false};
        if constexpr (detect)
        {
            // Detection of line type is enabled.
            // An alignment line can be created successfully only if:
            // 1) The line doesn't begin with SAMHeaderLine::getBeginChar().
            // 2) The line is parsed without error.
            if(line.front() != SAMHeaderLine::getBeginChar())
            {
                alignment_line = SAMAlignmentLineType(line, parse_align_line, parse_mand_align_fields, parse_opt_align_fields, parse_opt_align_fields_attribs, flush_ostream);
                status = true;
            }
        }
        else
        {
            // Detection of line type is disabled.
            // An alignment line can be created successfully if:
            // 1) The line is parsed without error.
            alignment_line = SAMAlignmentLineType(line, parse_align_line, parse_mand_align_fields, parse_opt_align_fields, parse_opt_align_fields_attribs, pref_opt_fields_tags, flush_ostream);
            status = true;
        }
        // Return the status of object creation.
        return status;
    }
};

}

#endif /* SAMFileReader_hpp */
