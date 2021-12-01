//
//  SAMAlignmentMandatoryFields.cpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 3/19/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#include <regex>
#include <utility>
#include <cstring>
#include <sstream>
#include <stdexcept>
#include <utk/StringUtils.hpp>
#include <hts/SAMAlignmentMandatoryFields.hpp>

namespace hts
{

/// Initialization of standard field and value regex of mandatory fields of
/// SAM format.
const SAMAlignmentMandatoryFields::SAMMandatoryFieldRegexes SAMAlignmentMandatoryFields::std_sam_align_mand_field_regexes =
{
    {"QNAME", "[!-?A-~]{1,254}"}, // Query template NAME
    {"RNAME", "\\*|[!-()+-<>-~][!-~]*"}, // Reference sequence NAME
    {"CIGAR", "\\*|([0-9]+[MIDNSHPX=])+"}, // CIGAR string
    {"RNEXT", "\\*|=|[!-()+-<>-~][!-~]*"}, // Reference name of the mate/next read
    {"SEQ", "\\*|[A-Za-z=\\.]+"}, // segment SEQuence
    {"QUAL", "[!-~]+"} // ASCII of Phred-scaled base QUALity+33
};

SAMAlignmentMandatoryFields::SAMAlignmentMandatoryFields() {}

SAMAlignmentMandatoryFields::SAMAlignmentMandatoryFields(const std::string& qname, std::size_t flag, const std::string& rname, std::size_t pos, std::size_t mapq, const std::string& cigar, const std::string& rnext, std::size_t pnext, long long tlen, const std::string& seq, const std::string& qual, bool parse_fields, bool flush_ostream) : qname{qname}, flag{flag}, rname{rname}, pos{pos}, mapq{mapq}, cigar{cigar}, rnext{rnext}, pnext{pnext}, tlen{tlen}, seq{seq}, qual{qual}, flush_ostream{flush_ostream}
{
    // Assign parse masks.
    if(parse_fields) parse_masks.set();
    else parse_masks.reset();
    // Parse all mandatory fields.
    parse();
}

SAMAlignmentMandatoryFields::SAMAlignmentMandatoryFields(std::string&& qname, std::size_t flag, std::string&& rname, std::size_t pos, std::size_t mapq, std::string&& cigar, std::string&& rnext, std::size_t pnext, long long tlen, std::string&& seq, std::string&& qual, bool parse_fields, bool flush_ostream) : qname{std::move(qname)}, flag{flag}, rname{std::move(rname)}, pos{pos}, mapq{mapq}, cigar{std::move(cigar)}, rnext{std::move(rnext)}, pnext{pnext}, tlen{tlen}, seq{std::move(seq)}, qual{std::move(qual)}, flush_ostream{flush_ostream}
{
    // Assign parse masks.
    if(parse_fields) parse_masks.set();
    else parse_masks.reset();
    // Parse all mandatory fields.
    parse();
}

SAMAlignmentMandatoryFields::SAMAlignmentMandatoryFields(const std::string& qname, std::size_t flag, const std::string& rname, std::size_t pos, std::size_t mapq, const std::string& cigar, const std::string& rnext, std::size_t pnext, long long tlen, const std::string& seq, const std::string& qual, const std::string& parse_fields, bool flush_ostream) : qname{qname}, flag{flag}, rname{rname}, pos{pos}, mapq{mapq}, cigar{cigar}, rnext{rnext}, pnext{pnext}, tlen{tlen}, seq{seq}, qual{qual}, flush_ostream{flush_ostream}
{
    // Assign parse masks.
    parse_masks = makeParseMasks(parse_fields);
    // Parse all mandatory fields.
    parse();
}

SAMAlignmentMandatoryFields::SAMAlignmentMandatoryFields(std::string&& qname, std::size_t flag, std::string&& rname, std::size_t pos, std::size_t mapq, std::string&& cigar, std::string&& rnext, std::size_t pnext, long long tlen, std::string&& seq, std::string&& qual, const std::string& parse_fields, bool flush_ostream) : qname{std::move(qname)}, flag{flag}, rname{std::move(rname)}, pos{pos}, mapq{mapq}, cigar{std::move(cigar)}, rnext{std::move(rnext)}, pnext{pnext}, tlen{tlen}, seq{std::move(seq)}, qual{std::move(qual)}, flush_ostream{flush_ostream}
{
    // Assign parse masks.
    parse_masks = makeParseMasks(parse_fields);
    // Parse all mandatory fields.
    parse();
}

SAMAlignmentMandatoryFields::SAMAlignmentMandatoryFields(const SAMAlignmentMandatoryFields& mand_fields) : qname{mand_fields.qname}, flag{mand_fields.flag}, rname{mand_fields.rname}, pos{mand_fields.pos}, mapq{mand_fields.mapq}, cigar{mand_fields.cigar}, rnext{mand_fields.rnext}, pnext{mand_fields.pnext}, tlen{mand_fields.tlen}, seq{mand_fields.seq}, qual{mand_fields.qual}, read_length{mand_fields.read_length}, parse_masks{mand_fields.parse_masks}, flush_ostream{mand_fields.flush_ostream} {}

SAMAlignmentMandatoryFields::SAMAlignmentMandatoryFields(SAMAlignmentMandatoryFields&& mand_fields) : qname{std::move(mand_fields.qname)}, flag{mand_fields.flag}, rname{std::move(mand_fields.rname)}, pos{mand_fields.pos}, mapq{mand_fields.mapq}, cigar{std::move(mand_fields.cigar)}, rnext{std::move(mand_fields.rnext)}, pnext{mand_fields.pnext}, tlen{mand_fields.tlen}, seq{std::move(mand_fields.seq)}, qual{std::move(mand_fields.qual)}, read_length{mand_fields.read_length}, parse_masks{std::move(mand_fields.parse_masks)}, flush_ostream{mand_fields.flush_ostream}
{
    mand_fields.reset();
}

SAMAlignmentMandatoryFields::~SAMAlignmentMandatoryFields() noexcept {}

SAMAlignmentMandatoryFields& SAMAlignmentMandatoryFields::operator=(const SAMAlignmentMandatoryFields& mand_fields)
{
    if(this != &mand_fields)
    {
        qname = mand_fields.qname;
        flag = mand_fields.flag;
        rname = mand_fields.rname;
        pos = mand_fields.pos;
        mapq = mand_fields.mapq;
        cigar = mand_fields.cigar;
        rnext = mand_fields.rnext;
        pnext = mand_fields.pnext;
        tlen = mand_fields.tlen;
        seq = mand_fields.seq;
        qual = mand_fields.qual;
        read_length = mand_fields.read_length;
        parse_masks = mand_fields.parse_masks;
        flush_ostream = mand_fields.flush_ostream;
    }
    return *this;
}

SAMAlignmentMandatoryFields& SAMAlignmentMandatoryFields::operator=(SAMAlignmentMandatoryFields&& mand_fields)
{
    if(this != &mand_fields)
    {
        qname = std::move(mand_fields.qname);
        flag = mand_fields.flag;
        rname = std::move(mand_fields.rname);
        pos = mand_fields.pos;
        mapq = mand_fields.mapq;
        cigar = std::move(mand_fields.cigar);
        rnext = std::move(mand_fields.rnext);
        pnext = mand_fields.pnext;
        tlen = mand_fields.tlen;
        seq = std::move(mand_fields.seq);
        qual = std::move(mand_fields.qual);
        read_length = mand_fields.read_length;
        parse_masks = std::move(mand_fields.parse_masks);
        flush_ostream = mand_fields.flush_ostream;
        mand_fields.reset();
    }
    return *this;
}

/// Make a bitset mask from a character string.
SAMAlignmentMandatoryFields::ParseMasks SAMAlignmentMandatoryFields::makeParseMasks(const std::string& parse_fields)
{
    if(std::size_t parse_fields_length = parse_fields.length(); parse_fields_length > 0)
    {
        // The bit-mask character string must have the same length as the bitset mask.
        if(std::size_t parse_masks_length = parse_masks.size(); parse_fields_length != parse_masks_length)
        {
            std::ostringstream err_msg;
            err_msg << qname << "The bit-mask character string for parsing the mandatory fields of SAM alignment line must have " << parse_masks_length << " characters if not empty!";
            throw std::logic_error(err_msg.str());
        }
        // Only 0 and 1 are allowed for the bit-mask character string.
        if(std::regex_search(parse_fields, std::regex("[^01]")))
        {
            throw std::logic_error("Only 0 and 1 are allowed in the bit-mask character string for parsing the mandatory fields of SAM alignment line!");
        }
    }
    return ParseMasks(parse_fields);
}

/// Clear all data members.
void SAMAlignmentMandatoryFields::reset()
{
    qname.clear();
    flag = 0;
    rname.clear();
    pos = 0;
    mapq = 0;
    cigar.clear();
    rnext.clear();
    pnext = 0;
    tlen = 0;
    seq.clear();
    qual.clear();
    read_length = 0;
    parse_masks.reset();
    flush_ostream = false;
}

/// Parse mandatory fields.
void SAMAlignmentMandatoryFields::parse()
{
    // Parse QNAME.
    if(parse_masks.test(0))
    {
        const std::string& qname_regex_str = std_sam_align_mand_field_regexes.at("QNAME");
        std::regex qname_regex(qname_regex_str);
        if(!std::regex_match(qname, qname_regex))
        {
            std::ostringstream err_msg;
            err_msg << qname << " doesn't match with mandatory pattern of QNAME: " << qname_regex_str << " !";
            throw std::logic_error(err_msg.str());
        }
    }

    /// Parse FLAG: [0, (2^16)-1]
    if(parse_masks.test(1))
    {
        if(flag > 65535)
        {
            std::ostringstream err_msg;
            err_msg << "FLAG " << flag << " is greater than 65535!";
            throw std::logic_error(err_msg.str());
        }
    }

    // Parse RNAME.
    if(parse_masks.test(2))
    {
        const std::string& rname_regex_str = std_sam_align_mand_field_regexes.at("RNAME");
        std::regex rname_regex(rname_regex_str);
        if(!std::regex_match(rname, rname_regex))
        {
            std::ostringstream err_msg;
            err_msg << rname << " doesn't match with mandatory pattern of RNAME: " << rname_regex_str << " !";
            throw std::logic_error(err_msg.str());
        }
    }

    /// Parse POS: [0, (2^31)-1]
    if(parse_masks.test(3))
    {
        if(pos > 2147483647)
        {
            std::ostringstream err_msg;
            err_msg << "POS " << pos << " is greater than 2147483647!";
            throw std::logic_error(err_msg.str());
        }
    }

    /// Parse MAPQ: [0, (2^8)-1]
    if(parse_masks.test(4))
    {
        if(mapq > 255)
        {
            std::ostringstream err_msg;
            err_msg << "MAPQ " << mapq << " is greater than 255!";
            throw std::logic_error(err_msg.str());
        }
    }

    // Parse CIGAR.
    if(parse_masks.test(5))
    {
        const std::string& cigar_regex_str = std_sam_align_mand_field_regexes.at("CIGAR");
        std::regex cigar_regex(cigar_regex_str);
        if(!std::regex_match(cigar, cigar_regex))
        {
            std::ostringstream err_msg;
            err_msg << cigar << " doesn't match with mandatory pattern of CIGAR: " << cigar_regex_str << " !";
            throw std::logic_error(err_msg.str());
        }
    }

    // Parse RNEXT.
    if(parse_masks.test(6))
    {
        const std::string& rnext_regex_str = std_sam_align_mand_field_regexes.at("RNEXT");
        std::regex rnext_regex(rnext_regex_str);
        if(!std::regex_match(rnext, rnext_regex))
        {
            std::ostringstream err_msg;
            err_msg << rnext << " doesn't match with mandatory pattern of RNEXT: " << rnext_regex_str << " !";
            throw std::logic_error(err_msg.str());
        }
    }

    /// Parse PNEXT: [0, (2^31)-1]
    if(parse_masks.test(7))
    {
        if(pnext > 2147483647)
        {
            std::ostringstream err_msg;
            err_msg << "PNEXT " << pnext << " is greater than 2147483647!";
            throw std::logic_error(err_msg.str());
        }
    }

    /// Parse TLEN: [-(2^31)+1, (2^31)-1]
    if(parse_masks.test(8))
    {
        if(tlen < -2147483647 || tlen > 2147483647)
        {
            std::ostringstream err_msg;
            err_msg << "TLEN " << tlen << " is greater than 2147483647!";
            throw std::logic_error(err_msg.str());
        }
    }

    // Parse SEQ.
    if(parse_masks.test(9))
    {
        const std::string& seq_regex_str = std_sam_align_mand_field_regexes.at("SEQ");
        std::regex seq_regex(seq_regex_str);
        if(!std::regex_match(seq, seq_regex))
        {
            std::ostringstream err_msg;
            err_msg << seq << " doesn't match with mandatory pattern of SEQ: " << seq_regex_str << " !";
            throw std::logic_error(err_msg.str());
        }
    }

    // Parse QUAL.
    if(parse_masks.test(10))
    {
        const std::string& qual_regex_str = std_sam_align_mand_field_regexes.at("QUAL");
        std::regex qual_regex(qual_regex_str);
        if(!std::regex_match(qual, qual_regex))
        {
            std::ostringstream err_msg;
            err_msg << qual << " doesn't match with mandatory pattern of QUAL: " << qual_regex_str << " !";
            throw std::logic_error(err_msg.str());
        }
    }

    // Equal length of SEQ and QUAL.
    if(parse_masks.test(11))
    {
        if(std::size_t seq_length = seq.length(); seq_length == qual.length()) read_length = seq_length;
        else
        {
            std::ostringstream err_msg;
            err_msg << "The lengths of " << seq << " and " << qual << " are different!";
            throw std::logic_error(err_msg.str());
        }
    }
}

/// Auxiliary function for generating printable output string.
std::string SAMAlignmentMandatoryFields::genOutputString() const
{
    std::ostringstream str_os;
    str_os << qname << tab_sep << flag << tab_sep << rname << tab_sep << pos << tab_sep << mapq << tab_sep << cigar << tab_sep << rnext << tab_sep << pnext << tab_sep << tlen << tab_sep << seq << tab_sep << qual;
    return str_os.str();
}

/// Print mandatory fields of SAM file.
std::ostream& operator<<(std::ostream& os, const SAMAlignmentMandatoryFields& mand_fields)
{
    os << mand_fields.genOutputString();
    if(mand_fields.getFlushOstream()) os.flush();
    return os;
}

}
