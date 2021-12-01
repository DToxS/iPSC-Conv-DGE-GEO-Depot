//
//  ConvIlluminaFASTQSequence.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 5/4/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef ConvIlluminaFASTQSequence_hpp
#define ConvIlluminaFASTQSequence_hpp

#include <vector>
#include "IlluminaFASTQSequence.hpp"

namespace hts
{

class ConvIlluminaFASTQSequence : public IlluminaFASTQSequence
{
protected:

    // Well barcode.
    std::string well_barcode;

protected:

    // Set the group ID of FASTQ sequence using sequence information.
    virtual void setGroupId() override;

    void setBarcode();

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
    
    ConvIlluminaFASTQSequence();
    
    // parse_seq is defaulted to true for the parsing of IlluminaFASTQSequence.
    ConvIlluminaFASTQSequence(const FASTQSequenceLines& lines, bool parse_seq=true, bool parse_seq_id_level_1=false, bool parse_seq_id_level_2=false, bool flush_ostream=false);
    
    // parse_seq is defaulted to true for the parsing of IlluminaFASTQSequence.
    ConvIlluminaFASTQSequence(FASTQSequenceLines&& lines, bool parse_seq=true, bool parse_seq_id_level_1=false, bool parse_seq_id_level_2=false, bool flush_ostream=false);

    // parse_seq is defaulted to true for the parsing of IlluminaFASTQSequence.
    ConvIlluminaFASTQSequence(const std::string& line1, const std::string& line2, const std::string& line3, const std::string& line4, bool parse_seq=true, bool parse_seq_id_level_1=false, bool parse_seq_id_level_2=false, bool flush_ostream=false);

    // parse_seq is defaulted to true for the parsing of IlluminaFASTQSequence.
    ConvIlluminaFASTQSequence(std::string&& line1, std::string&& line2, std::string&& line3, std::string&& line4, bool parse_seq=true, bool parse_seq_id_level_1=false, bool parse_seq_id_level_2=false, bool flush_ostream=false);

    ConvIlluminaFASTQSequence(const IlluminaFASTQSequence& seq);

    ConvIlluminaFASTQSequence(IlluminaFASTQSequence&& seq);

    ConvIlluminaFASTQSequence(const FASTQSequence& seq, bool parse_seq_id_level_1=false, bool parse_seq_id_level_2=false);

    ConvIlluminaFASTQSequence(FASTQSequence&& seq, bool parse_seq_id_level_1=false, bool parse_seq_id_level_2=false);

    ConvIlluminaFASTQSequence(const ConvIlluminaFASTQSequence& seq);

    ConvIlluminaFASTQSequence(ConvIlluminaFASTQSequence&& seq);

    virtual ~ConvIlluminaFASTQSequence() noexcept;
    
    ConvIlluminaFASTQSequence& operator=(const ConvIlluminaFASTQSequence& seq);
    
    ConvIlluminaFASTQSequence& operator=(ConvIlluminaFASTQSequence&& seq);
    
    // Retrieve the well barcode of Conv FASTQ sequence.
    const std::string& getWellBarcode() const
    {
        return well_barcode;
    }

    // Retrieve the well barcode of Conv FASTQ sequence.
    std::string& getWellBarcode()
    {
        return well_barcode;
    }
};

using ConvIlluminaFASTQSequences = std::vector<ConvIlluminaFASTQSequence>;

}

#endif /* ConvIlluminaFASTQSequence_hpp */
