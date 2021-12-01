//
//  SAMAlignmentMandatoryFields.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 3/19/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#ifndef SAMAlignmentMandatoryFields_hpp
#define SAMAlignmentMandatoryFields_hpp

#include <map>
#include <bitset>
#include <string>
#include <ostream>
#include <cstddef>

namespace hts
{

/// \brief The structure of an alignment line of a standard SAM file.
/// This class contains all mandatory fields of an alignment line defined by
/// the document:
///
/// Sequence Alignment/Map Format Specification
/// The SAM/BAM Format Specification Working Group
/// June 1, 2017
class SAMAlignmentMandatoryFields
{
public:

    /// Data type for standard field and value regex of mandatory fields of
    /// SAM format.
    using SAMMandatoryFieldRegexes = std::map<std::string, std::string>;
    static const SAMMandatoryFieldRegexes std_sam_align_mand_field_regexes;

private:

    /// The separator of multiple mandatory fields.
    static constexpr char tab_sep {'\t'};

    /// Number of mandatory alignment fields of a SAM file.
    static constexpr const std::size_t n_mand_fields {11};

    /// Type of the indicators of parsing mandatory fields.
    /// Note: one more mask bit for testing the length equality between SEQ and
    /// QUAL fields.
    using ParseMasks = std::bitset<n_mand_fields+1>;

protected:

    /// 11 mandatory alignment fields by SAM standard.

    /// QNAME: Query template NAME
    std::string qname;
    /// FLAG: bitwise FLAG
    std::size_t flag {0};
    /// RNAME: Reference sequence NAME
    std::string rname;
    /// POS: 1-based leftmost mapping POSition
    std::size_t pos {0};
    /// MAPQ: MAPping Quality
    std::size_t mapq {0};
    /// CIGAR: CIGAR string
    std::string cigar;
    /// RNEXT: Reference name of the mate/next read
    std::string rnext;
    /// PNEXT: Position of the mate/next read
    std::size_t pnext {0};
    /// TLEN: observed Template LENgth
    long long tlen {0};
    /// SEQ: segment SEQuence
    std::string seq;
    /// QUAL: ASCII of Phred-scaled base QUALity+33
    std::string qual;

    // Read length of SAM sequence.
    std::size_t read_length {0};

    /// Bit masks for the indicator of parsing each mandatory fields.
    ParseMasks parse_masks;

    /// Flush each written sequence line from output stream to disk.
    /// Note: this option needs to be switched on in a multi-threading envronment.
    bool flush_ostream {false};

private:

    /// Make a bitset mask from a character string.
    ParseMasks makeParseMasks(const std::string& parse_fields);

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

    SAMAlignmentMandatoryFields();

    SAMAlignmentMandatoryFields(const std::string& qname, std::size_t flag, const std::string& rname, std::size_t pos, std::size_t mapq, const std::string& cigar, const std::string& rnext, std::size_t pnext, long long tlen, const std::string& seq, const std::string& qual, bool parse_fields=false, bool flush_ostream=false);
    
    SAMAlignmentMandatoryFields(std::string&& qname, std::size_t flag, std::string&& rname, std::size_t pos, std::size_t mapq, std::string&& cigar, std::string&& rnext, std::size_t pnext, long long tlen, std::string&& seq, std::string&& qual, bool parse_fields=false, bool flush_ostream=false);

    SAMAlignmentMandatoryFields(const std::string& qname, std::size_t flag, const std::string& rname, std::size_t pos, std::size_t mapq, const std::string& cigar, const std::string& rnext, std::size_t pnext, long long tlen, const std::string& seq, const std::string& qual, const std::string& parse_fields="", bool flush_ostream=false);

    SAMAlignmentMandatoryFields(std::string&& qname, std::size_t flag, std::string&& rname, std::size_t pos, std::size_t mapq, std::string&& cigar, std::string&& rnext, std::size_t pnext, long long tlen, std::string&& seq, std::string&& qual, const std::string& parse_fields="", bool flush_ostream=false);

    SAMAlignmentMandatoryFields(const SAMAlignmentMandatoryFields& mand_fields);

    SAMAlignmentMandatoryFields(SAMAlignmentMandatoryFields&& mand_fields);

    virtual ~SAMAlignmentMandatoryFields() noexcept;

    SAMAlignmentMandatoryFields& operator=(const SAMAlignmentMandatoryFields& mand_fields);

    SAMAlignmentMandatoryFields& operator=(SAMAlignmentMandatoryFields&& mand_fields);

    /// Check if mandatory field is empty using QNAME.
    bool empty() const
    {
        return qname.empty();
    }

    static const SAMMandatoryFieldRegexes& getStdSAMAlignmentMandatoryFieldRegexes()
    {
        return std_sam_align_mand_field_regexes;
    }

    static char getSeparator()
    {
        return tab_sep;
    }

    static std::size_t getNumberOfMandatoryFields()
    {
        return n_mand_fields;
    }

    const std::string& getQName() const
    {
        return qname;
    }

    std::size_t getFlag() const
    {
        return flag;
    }

    const std::string& getRName() const
    {
        return rname;
    }

    std::size_t getPos() const
    {
        return pos;
    }

    std::size_t getMapQ() const
    {
        return mapq;
    }

    const std::string& getCigar() const
    {
        return cigar;
    }

    const std::string& getRNext() const
    {
        return rnext;
    }

    std::size_t getPNext() const
    {
        return pnext;
    }

    long long getTLen() const
    {
        return tlen;
    }

    const std::string& getSeq() const
    {
        return seq;
    }

    const std::string& getQual() const
    {
        return qual;
    }

    std::size_t getReadLength() const
    {
        return read_length;
    }

    const ParseMasks& getParseMasks() const
    {
        return parse_masks;
    }

    bool getFlushOstream() const
    {
        return flush_ostream;
    }

    /// Parse mandatory fields.
    void parse();

    /// Auxiliary function for generating printable output string.
    std::string genOutputString() const;
};

/// Print mandatory fields of SAM file.
std::ostream& operator<<(std::ostream& os, const SAMAlignmentMandatoryFields& mand_field);

}

#endif /* SAMAlignmentMandatoryFields_hpp */
