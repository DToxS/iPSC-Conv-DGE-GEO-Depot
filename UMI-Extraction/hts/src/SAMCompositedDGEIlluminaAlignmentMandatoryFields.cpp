//
//  SAMAlignmentCompositedDGEIlluminaMandatoryFields.cpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 3/29/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#include <utk/StringUtils.hpp>
#include <hts/SAMCompositedDGEIlluminaAlignmentMandatoryFields.hpp>
#include <hts/CompositedDGEIlluminaFASTQSequence.hpp>

namespace hts
{

SAMCompositedDGEIlluminaAlignmentMandatoryFields::SAMCompositedDGEIlluminaAlignmentMandatoryFields() : SAMAlignmentMandatoryFields() {}

SAMCompositedDGEIlluminaAlignmentMandatoryFields::SAMCompositedDGEIlluminaAlignmentMandatoryFields(const std::string& qname, std::size_t flag, const std::string& rname, std::size_t pos, std::size_t mapq, const std::string& cigar, const std::string& rnext, std::size_t pnext, long long tlen, const std::string& seq, const std::string& qual, bool parse_fields, bool flush_ostream) : SAMAlignmentMandatoryFields(qname, flag, rname, pos, mapq, cigar, rnext, pnext, tlen, seq, qual, parse_fields, flush_ostream)
{
    // Parse mandatory fields.
    parse();
}

SAMCompositedDGEIlluminaAlignmentMandatoryFields::SAMCompositedDGEIlluminaAlignmentMandatoryFields(std::string&& qname, std::size_t flag, std::string&& rname, std::size_t pos, std::size_t mapq, std::string&& cigar, std::string&& rnext, std::size_t pnext, long long tlen, std::string&& seq, std::string&& qual, bool parse_fields, bool flush_ostream) : SAMAlignmentMandatoryFields(std::move(qname), flag, std::move(rname), pos, mapq, std::move(cigar), std::move(rnext), pnext, tlen, std::move(seq), std::move(qual), parse_fields, flush_ostream)
{
    // Parse mandatory fields.
    parse();
}

SAMCompositedDGEIlluminaAlignmentMandatoryFields::SAMCompositedDGEIlluminaAlignmentMandatoryFields(const std::string& qname, std::size_t flag, const std::string& rname, std::size_t pos, std::size_t mapq, const std::string& cigar, const std::string& rnext, std::size_t pnext, long long tlen, const std::string& seq, const std::string& qual, const std::string& parse_fields, bool flush_ostream) : SAMAlignmentMandatoryFields(qname, flag, rname, pos, mapq, cigar, rnext, pnext, tlen, seq, qual, parse_fields, flush_ostream)
{
    // Parse mandatory fields.
    parse();
}

SAMCompositedDGEIlluminaAlignmentMandatoryFields::SAMCompositedDGEIlluminaAlignmentMandatoryFields(std::string&& qname, std::size_t flag, std::string&& rname, std::size_t pos, std::size_t mapq, std::string&& cigar, std::string&& rnext, std::size_t pnext, long long tlen, std::string&& seq, std::string&& qual, const std::string& parse_fields, bool flush_ostream) : SAMAlignmentMandatoryFields(std::move(qname), flag, std::move(rname), pos, mapq, std::move(cigar), std::move(rnext), pnext, tlen, std::move(seq), std::move(qual), parse_fields, flush_ostream)
{
    // Parse mandatory fields.
    parse();
}

SAMCompositedDGEIlluminaAlignmentMandatoryFields::SAMCompositedDGEIlluminaAlignmentMandatoryFields(const SAMCompositedDGEIlluminaAlignmentMandatoryFields& mand_fields) : SAMAlignmentMandatoryFields(mand_fields) {}

SAMCompositedDGEIlluminaAlignmentMandatoryFields::SAMCompositedDGEIlluminaAlignmentMandatoryFields(SAMCompositedDGEIlluminaAlignmentMandatoryFields&& mand_fields) : SAMAlignmentMandatoryFields(std::move(mand_fields)) {}

SAMCompositedDGEIlluminaAlignmentMandatoryFields::~SAMCompositedDGEIlluminaAlignmentMandatoryFields() noexcept {}

SAMCompositedDGEIlluminaAlignmentMandatoryFields& SAMCompositedDGEIlluminaAlignmentMandatoryFields::operator=(const SAMCompositedDGEIlluminaAlignmentMandatoryFields& mand_fields)
{
    if(this != &mand_fields)
    {
        SAMAlignmentMandatoryFields::operator=(mand_fields);
    }
    return *this;
}

SAMCompositedDGEIlluminaAlignmentMandatoryFields& SAMCompositedDGEIlluminaAlignmentMandatoryFields::operator=(SAMCompositedDGEIlluminaAlignmentMandatoryFields&& mand_fields)
{
    if(this != &mand_fields)
    {
        SAMAlignmentMandatoryFields::operator=(std::move(mand_fields));
    }
    return *this;
}

/// Parse mandatory fields.
void SAMCompositedDGEIlluminaAlignmentMandatoryFields::parse()
{
    // Parse QNAME according to the sequence ID line of the composite DGE
    // Illimina FASTQ sequence.
    if(parse_masks.test(0))
    {
        std::vector<std::string> parts = utk::splitString(qname, IlluminaFASTQSequence::colon_sep);
        if(parts.size() != CompositedDGEIlluminaFASTQSequence::n_seq_id_parts)
        {
            std::ostringstream err_msg;
            err_msg << "The QNAME of the mandatory fields of a SAM alignment line for a composite 3'-DGE Illumina FASTQ sequence must have " << CompositedDGEIlluminaFASTQSequence::n_seq_id_parts << " parts separated by " << IlluminaFASTQSequence::colon_sep << " character!";
            throw std::logic_error(err_msg.str());
        }
    }
}

}
