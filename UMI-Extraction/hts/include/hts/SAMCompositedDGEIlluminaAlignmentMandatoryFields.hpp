//
//  SAMCompositedDGEIlluminaAlignmentMandatoryFields.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 3/29/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#ifndef SAMCompositedDGEIlluminaAlignmentMandatoryFields_hpp
#define SAMCompositedDGEIlluminaAlignmentMandatoryFields_hpp

#include <string>
#include "SAMAlignmentMandatoryFields.hpp"

namespace hts
{

/// \brief A 3'-DGE version of mandatory fields of SAM alignment line.
/// This class defines the type of mandatory fields of SAM alignment line for
/// the CompositedDGEIlluminaFASTQSequence, by enforcing a specific QNAME
/// format that matches with the sequence ID line of the composite DGE Illimina
/// FASTQ sequence.
class SAMCompositedDGEIlluminaAlignmentMandatoryFields : public SAMAlignmentMandatoryFields
{
public:

    SAMCompositedDGEIlluminaAlignmentMandatoryFields();

    SAMCompositedDGEIlluminaAlignmentMandatoryFields(const std::string& qname, std::size_t flag, const std::string& rname, std::size_t pos, std::size_t mapq, const std::string& cigar, const std::string& rnext, std::size_t pnext, long long tlen, const std::string& seq, const std::string& qual, bool parse_fields=false, bool flush_ostream=false);

    SAMCompositedDGEIlluminaAlignmentMandatoryFields(std::string&& qname, std::size_t flag, std::string&& rname, std::size_t pos, std::size_t mapq, std::string&& cigar, std::string&& rnext, std::size_t pnext, long long tlen, std::string&& seq, std::string&& qual, bool parse_fields=false, bool flush_ostream=false);

    SAMCompositedDGEIlluminaAlignmentMandatoryFields(const std::string& qname, std::size_t flag, const std::string& rname, std::size_t pos, std::size_t mapq, const std::string& cigar, const std::string& rnext, std::size_t pnext, long long tlen, const std::string& seq, const std::string& qual, const std::string& parse_fields="", bool flush_ostream=false);

    SAMCompositedDGEIlluminaAlignmentMandatoryFields(std::string&& qname, std::size_t flag, std::string&& rname, std::size_t pos, std::size_t mapq, std::string&& cigar, std::string&& rnext, std::size_t pnext, long long tlen, std::string&& seq, std::string&& qual, const std::string& parse_fields="", bool flush_ostream=false);

    SAMCompositedDGEIlluminaAlignmentMandatoryFields(const SAMCompositedDGEIlluminaAlignmentMandatoryFields& mand_fields);

    SAMCompositedDGEIlluminaAlignmentMandatoryFields(SAMCompositedDGEIlluminaAlignmentMandatoryFields&& mand_fields);

    virtual ~SAMCompositedDGEIlluminaAlignmentMandatoryFields() noexcept;

    SAMCompositedDGEIlluminaAlignmentMandatoryFields& operator=(const SAMCompositedDGEIlluminaAlignmentMandatoryFields& mand_fields);

    SAMCompositedDGEIlluminaAlignmentMandatoryFields& operator=(SAMCompositedDGEIlluminaAlignmentMandatoryFields&& mand_fields);

    /// Parse mandatory fields.
    void parse();
};

}

#endif /* SAMCompositedDGEIlluminaAlignmentMandatoryFields_hpp */
