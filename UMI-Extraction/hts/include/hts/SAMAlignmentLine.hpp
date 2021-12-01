//
//  SAMAlignmentLine.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 3/20/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#ifndef SAMAlignmentLine_hpp
#define SAMAlignmentLine_hpp

#include <regex>
#include <utility>
#include <string>
#include <vector>
#include <type_traits>
#include <utk/StringUtils.hpp>
#include "SAMAlignmentMandatoryFields.hpp"
#include "SAMAlignmentOptionalFields.hpp"

namespace hts
{

/// \brief The structure of an alignment line of a SAM file.
/// This class defines the structure of an alignment line contained in the
/// alignment section of a SAM file, as specified by the document:
///
/// Sequence Alignment/Map Format Specification
/// The SAM/BAM Format Specification Working Group
/// June 1, 2017
///
/// \tparam  SAMAlignmentMandatoryFieldsType   A type of SAMAlignmentMandatoryFields-based mandatory field list of SAM alignment line.
/// \tparam  SAMAlignmentOptionalFieldsType   A type of SAMAlignmentOptionalFields-based optional field list of SAM alignment line.
/// \tparam  SAMAlignmentOptionalFieldType   A type of SAMAlignmentOptionalField-based optional field of SAM alignment line.
template<typename SAMAlignmentMandatoryFieldsType, typename SAMAlignmentOptionalFieldsType, typename SAMAlignmentOptionalFieldType>
class SAMAlignmentLine
{
protected:

    /// The separator between mandatory and optional alignment fields.
    static constexpr char tab_sep {'\t'};

protected:

    /// Buffer for entire SAM alignment line.
    std::string line;

    /// Mandatory fields of SAM alignment line.
    SAMAlignmentMandatoryFieldsType mand_fields;

    /// Optional fields of SAM alignment line.
    SAMAlignmentOptionalFieldsType opt_fields;

    /// Indicator for validating top-level structure of SAM alignment line.
    /// Note: mandatory and optional fields will be assigned during parsing.
    bool parse_line {false};

    /// Indicator for validating all mandatory fields of SAM alignment line
    /// according to the SAM standard.
    bool parse_mand_fields {false};

    /// Indicator for validating top structure of each optional field of SAM
    /// alignment line.
    /// Note: the tag, type, and value of each optional field will be assigned
    ///       during parsing.
    bool parse_opt_fields {false};

    /// Indicator for validating the tag, type, and value attributes of optional
    /// field of SAM alignment line according to the SAM standard.
    bool parse_opt_fields_attribs {false};

    /// The tags of preferred optional fields to be parsed.
    /// If not empty, these preferred optional fields will be parsed and others
    /// won't.
    SAMAlignmentOptionalFieldParts pref_opt_fields_tags;

    /// \brief Flush each written alignment line from output stream to disk.
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
        line.clear();
        opt_fields.clear();
        parse_line = false;
        parse_mand_fields = false;
        parse_opt_fields = false;
        parse_opt_fields_attribs = false;
        pref_opt_fields_tags.clear();
        flush_ostream = false;
    }

public:

    SAMAlignmentLine()
    {
        if constexpr (!std::is_base_of_v<SAMAlignmentMandatoryFields, SAMAlignmentMandatoryFieldsType> || !std::is_base_of_v<SAMAlignmentOptionalFields, SAMAlignmentOptionalFieldsType> || !std::is_base_of_v<SAMAlignmentOptionalField, SAMAlignmentOptionalFieldType>) throw std::logic_error("SAMAlignmentLine only accepts template parameters based on SAMAlignmentMandatoryFields, SAMAlignmentOptionalField, and SAMAlignmentOptionalFields!");
    }

    SAMAlignmentLine(const SAMAlignmentMandatoryFieldsType& mand_fields_val, const SAMAlignmentOptionalFieldsType& opt_fields_val, bool flush_ostream=false) : mand_fields{mand_fields_val}, opt_fields{opt_fields_val}, flush_ostream{flush_ostream}
    {
        if constexpr (std::is_base_of_v<SAMAlignmentMandatoryFields, SAMAlignmentMandatoryFieldsType> && std::is_base_of_v<SAMAlignmentOptionalFields, SAMAlignmentOptionalFieldsType> && std::is_base_of_v<SAMAlignmentOptionalField, SAMAlignmentOptionalFieldType>)
        {
            // Assign mandatory fields to line.
            line = mand_fields.genOutputString();
            // Add optional fields to line.
            for(const auto& opt_field : opt_fields) line += tab_sep + opt_field.getField();
        }
        else throw std::logic_error("SAMAlignmentLine only accepts template parameters based on SAMAlignmentMandatoryFields, SAMAlignmentOptionalField, and SAMAlignmentOptionalFields!");
    }

    SAMAlignmentLine(SAMAlignmentMandatoryFieldsType&& mand_fields_val, SAMAlignmentOptionalFieldsType&& opt_fields_val, bool flush_ostream=false) : mand_fields{std::move(mand_fields_val)}, opt_fields{std::move(opt_fields_val)}, flush_ostream{flush_ostream}
    {
        if constexpr (std::is_base_of_v<SAMAlignmentMandatoryFields, SAMAlignmentMandatoryFieldsType> && std::is_base_of_v<SAMAlignmentOptionalFields, SAMAlignmentOptionalFieldsType> && std::is_base_of_v<SAMAlignmentOptionalField, SAMAlignmentOptionalFieldType>)
        {
            // Assign mandatory fields to line.
            line = mand_fields.genOutputString();
            // Add optional fields to line.
            for(const auto& opt_field : opt_fields) line += tab_sep + opt_field.getField();
        }
        else throw std::logic_error("SAMAlignmentLine only accepts template parameters based on SAMAlignmentMandatoryFields, SAMAlignmentOptionalField, and SAMAlignmentOptionalFields!");
    }

    SAMAlignmentLine(const std::string& line_val, bool parse_line=true, bool parse_mand_fields=false, bool parse_opt_fields=true, bool parse_opt_fields_attribs=false, const SAMAlignmentOptionalFieldParts& pref_opt_fields_tags=SAMAlignmentOptionalFieldParts(), bool flush_ostream=false) : line{line_val}, parse_line{parse_line}, parse_mand_fields{parse_mand_fields}, parse_opt_fields{parse_opt_fields}, parse_opt_fields_attribs{parse_opt_fields_attribs}, pref_opt_fields_tags{pref_opt_fields_tags}, flush_ostream{flush_ostream}
    {
        if constexpr (std::is_base_of_v<SAMAlignmentMandatoryFields, SAMAlignmentMandatoryFieldsType> && std::is_base_of_v<SAMAlignmentOptionalFields, SAMAlignmentOptionalFieldsType> && std::is_base_of_v<SAMAlignmentOptionalField, SAMAlignmentOptionalFieldType>)
        {
            // Parse line and assign mandatory fields and optional fields.
            if(parse_line) parseLine();
        }
        else throw std::logic_error("SAMAlignmentLine only accepts template parameters based on SAMAlignmentMandatoryFields, SAMAlignmentOptionalField, and SAMAlignmentOptionalFields!");
    }

    SAMAlignmentLine(std::string&& line_val, bool parse_line=true, bool parse_mand_fields=false, bool parse_opt_fields=true, bool parse_opt_fields_attribs=false, SAMAlignmentOptionalFieldParts&& pref_opt_fields_tags=SAMAlignmentOptionalFieldParts(), bool flush_ostream=false) : line{std::move(line_val)}, parse_line{parse_line}, parse_mand_fields{parse_mand_fields}, parse_opt_fields{parse_opt_fields}, parse_opt_fields_attribs{parse_opt_fields_attribs}, pref_opt_fields_tags{std::move(pref_opt_fields_tags)}, flush_ostream{flush_ostream}
    {
        if constexpr (std::is_base_of_v<SAMAlignmentMandatoryFields, SAMAlignmentMandatoryFieldsType> && std::is_base_of_v<SAMAlignmentOptionalFields, SAMAlignmentOptionalFieldsType> && std::is_base_of_v<SAMAlignmentOptionalField, SAMAlignmentOptionalFieldType>)
        {
            // Parse line and assign mandatory fields and optional fields.
            if(parse_line) parseLine();
        }
        else throw std::logic_error("SAMAlignmentLine only accepts template parameters based on SAMAlignmentMandatoryFields, SAMAlignmentOptionalField, and SAMAlignmentOptionalFields!");
    }

    SAMAlignmentLine(const SAMAlignmentLine& align_line) : line{align_line.line}, mand_fields{align_line.mand_fields}, opt_fields{align_line.opt_fields}, parse_line{align_line.parse_line}, parse_mand_fields{align_line.parse_mand_fields}, parse_opt_fields{align_line.parse_opt_fields}, parse_opt_fields_attribs{align_line.parse_opt_fields_attribs}, flush_ostream{align_line.flush_ostream}
    {
        if constexpr (!std::is_base_of_v<SAMAlignmentMandatoryFields, SAMAlignmentMandatoryFieldsType> || !std::is_base_of_v<SAMAlignmentOptionalFields, SAMAlignmentOptionalFieldsType> || !std::is_base_of_v<SAMAlignmentOptionalField, SAMAlignmentOptionalFieldType>) throw std::logic_error("SAMAlignmentLine only accepts template parameters based on SAMAlignmentMandatoryFields, SAMAlignmentOptionalField, and SAMAlignmentOptionalFields!");
    }

    SAMAlignmentLine(SAMAlignmentLine&& align_line) : line{std::move(align_line.line)}, mand_fields{std::move(align_line.mand_fields)}, opt_fields{std::move(align_line.opt_fields)}, parse_line{align_line.parse_line}, parse_mand_fields{align_line.parse_mand_fields}, parse_opt_fields{align_line.parse_opt_fields}, parse_opt_fields_attribs{align_line.parse_opt_fields_attribs}, flush_ostream{align_line.flush_ostream}
    {
        if constexpr (std::is_base_of_v<SAMAlignmentMandatoryFields, SAMAlignmentMandatoryFieldsType> && std::is_base_of_v<SAMAlignmentOptionalFields, SAMAlignmentOptionalFieldsType> && std::is_base_of_v<SAMAlignmentOptionalField, SAMAlignmentOptionalFieldType>) align_line.reset();
        else throw std::logic_error("SAMAlignmentLine only accepts template parameters based on SAMAlignmentMandatoryFields, SAMAlignmentOptionalField, and SAMAlignmentOptionalFields!");
    }

    virtual ~SAMAlignmentLine() noexcept {}

    SAMAlignmentLine& operator=(const SAMAlignmentLine& align_line)
    {
        if(this != &align_line)
        {
            line = align_line.line;
            mand_fields = align_line.mand_fields;
            opt_fields = align_line.opt_fields;
            parse_line = align_line.parse_line;
            parse_mand_fields = align_line.parse_mand_fields;
            parse_opt_fields = align_line.parse_opt_fields;
            parse_opt_fields_attribs = align_line.parse_opt_fields_attribs;
            pref_opt_fields_tags = align_line.pref_opt_fields_tags;
            flush_ostream = align_line.flush_ostream;
        }
        return *this;
    }

    SAMAlignmentLine& operator=(SAMAlignmentLine&& align_line)
    {
        if(this != &align_line)
        {
            line = std::move(align_line.line);
            mand_fields = std::move(align_line.mand_fields);
            opt_fields = std::move(align_line.opt_fields);
            parse_line = align_line.parse_line;
            parse_mand_fields = align_line.parse_mand_fields;
            parse_opt_fields = align_line.parse_opt_fields;
            parse_opt_fields_attribs = align_line.parse_opt_fields_attribs;
            pref_opt_fields_tags = std::move(align_line.pref_opt_fields_tags);
            flush_ostream = align_line.flush_ostream;
            align_line.reset();
        }
        return *this;
    }

    /// Check if alignment line is empty.
    bool empty() const
    {
        return line.empty();
    }

    static char getSeparator()
    {
        return tab_sep;
    }

    /// Get the buffer for entire SAM alignment line.
    const std::string& getLine() const
    {
        return line;
    }

    /// Get mandatory fields of SAM alignment line.
    const SAMAlignmentMandatoryFieldsType& getMandatoryFields() const
    {
        return mand_fields;
    }

    /// Get optional fields of SAM alignment line.
    const SAMAlignmentOptionalFieldsType& getOptionalFields() const
    {
        return opt_fields;
    }

    /// Get the indicator of parse line.
    bool getParseLine() const
    {
        return parse_line;
    }

    /// Get the indicator of flushing output stream.
    bool getFlushOstream() const
    {
        return flush_ostream;
    }

    /// Parse top-level structure of alignment line.
    void parseLine()
    {
        std::vector<std::string> parts = utk::splitString(line, tab_sep);
        std::size_t n_parts = parts.size();
        if(n_parts >= SAMAlignmentMandatoryFieldsType::getNumberOfMandatoryFields())
        {
            auto it = parts.cbegin();

            // Assign 11 mandatory fields.
            std::string qname, rname, cigar, rnext, seq, qual;
            std::size_t flag{0}, pos{0}, mapq{0}, pnext{0};
            long long tlen{0};
            // Assign QNAME.
            qname = *(it++);
            if(qname.length() == 0) throw std::logic_error("QNAME is empty!");
            // Assign FLAG.
            try
            {
                flag = utk::convert<std::size_t>(*(it++));
            }
            catch(const std::logic_error& e)
            {
                throw std::logic_error("Failed to convert FLAG to std::size_t type!");
            }
            // Assign RNAME.
            rname = *(it++);
            if(qname.length() == 0) throw std::logic_error("RNAME is empty!");
            // Assign POS.
            try
            {
                pos = utk::convert<std::size_t>(*(it++));
            }
            catch(const std::logic_error& e)
            {
                throw std::logic_error("Failed to convert POS to std::size_t type!");
            }
            // Assign MAPQ.
            try
            {
                mapq = utk::convert<std::size_t>(*(it++));
            }
            catch(const std::logic_error& e)
            {
                throw std::logic_error("Failed to convert MAPQ to std::size_t type!");
            }
            // Assign CIGAR.
            cigar = *(it++);
            if(qname.length() == 0) throw std::logic_error("CIGAR is empty!");
            // Assign RNEXT.
            rnext = *(it++);
            if(qname.length() == 0) throw std::logic_error("RNEXT is empty!");
            // Assign PNEXT.
            try
            {
                pnext = utk::convert<std::size_t>(*(it++));
            }
            catch(const std::logic_error& e)
            {
                throw std::logic_error("Failed to convert PNEXT to std::size_t type!");
            }
            // Assign TLEN.
            try
            {
                tlen = utk::convert<long long>(*(it++));
            }
            catch(const std::logic_error& e)
            {
                throw std::logic_error("Failed to convert TLEN to long long type!");
            }
            // Assign SEQ.
            seq = *(it++);
            if(qname.length() == 0) throw std::logic_error("SEQ is empty!");
            // Assign QUAL.
            qual = *(it++);
            if(qname.length() == 0) throw std::logic_error("QUAL is empty!");
            // Assign the mandatory fields object.
            mand_fields = SAMAlignmentMandatoryFieldsType(qname, flag, rname, pos, mapq, cigar, rnext, pnext, tlen, seq, qual, parse_mand_fields, flush_ostream);

            // Assign the rest parts to optional fields.
            for(; it != parts.cend(); ++it)
            {
                if(pref_opt_fields_tags.size() > 0)
                {
                    // Only parse preferred optional fields.
                    for(const auto& tag : pref_opt_fields_tags)
                    {
                        if(it->substr(0,tag.length()) == tag)
                        {
                            // Find and parse optional field.
                            opt_fields.push_back(SAMAlignmentOptionalFieldType(*it, parse_opt_fields, parse_opt_fields_attribs, parse_opt_fields_attribs, parse_opt_fields_attribs, flush_ostream));
                        }
                        else
                        {
                            // Not find and skip parsing optional field.
                            opt_fields.push_back(SAMAlignmentOptionalFieldType(*it, false, false, false, false, flush_ostream));
                        }
                    }
                }
                else
                {
                    // Otherwise parse all optional fields.
                    opt_fields.push_back(SAMAlignmentOptionalFieldType(*it, parse_opt_fields, parse_opt_fields_attribs, parse_opt_fields_attribs, parse_opt_fields_attribs, flush_ostream));
                }
            }
        }
        else
        {
            std::ostringstream err_msg;
            err_msg << "Alignment line must have all " << SAMAlignmentMandatoryFieldsType::getNumberOfMandatoryFields() << " mandatory fields!";
            throw std::logic_error(err_msg.str());
        }
    }
};

/// Type of SAMAlignmentLine list.
template<typename SAMAlignmentMandatoryFieldsType, typename SAMAlignmentOptionalFieldType, typename SAMAlignmentOptionalFieldsType>
using SAMAlignmentLines = std::vector<SAMAlignmentLine<SAMAlignmentMandatoryFieldsType, SAMAlignmentOptionalFieldType, SAMAlignmentOptionalFieldsType>>;

/// Print an alignment line of SAM file.
template<typename SAMAlignmentMandatoryFieldsType, typename SAMAlignmentOptionalFieldType, typename SAMAlignmentOptionalFieldsType>
std::ostream& operator<<(std::ostream& os, const SAMAlignmentLine<SAMAlignmentMandatoryFieldsType, SAMAlignmentOptionalFieldType,SAMAlignmentOptionalFieldsType>& alignment_line)
{
    os << alignment_line.getLine();
    if(alignment_line.getFlushOstream()) os.flush();
    return os;
}

}

#endif /* SAMAlignmentLine_hpp */
