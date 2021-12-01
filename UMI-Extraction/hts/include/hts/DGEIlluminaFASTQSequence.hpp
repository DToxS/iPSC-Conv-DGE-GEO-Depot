//
//  DGEIlluminaFASTQSequence.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 5/4/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef DGEIlluminaFASTQSequence_hpp
#define DGEIlluminaFASTQSequence_hpp

#include <vector>
#include "IlluminaFASTQSequence.hpp"

namespace hts
{

class DGEIlluminaFASTQSequence : public IlluminaFASTQSequence
{
public:

    // Well barcode of DGE FASTQ sequence produced by Illumina.
    static const LineType well_barcode_line {Sequence};
    static const std::size_t well_barcode_beg_pos {0}, well_barcode_end_pos {5}, well_barcode_length {well_barcode_end_pos-well_barcode_beg_pos+1};

    // UMI barcode of DGE FASTQ sequence produced by Illumina.
    static const LineType umi_barcode_line {Sequence};
    static const std::size_t umi_barcode_beg_pos {6}, umi_barcode_end_pos {15}, umi_barcode_length {umi_barcode_end_pos-umi_barcode_beg_pos+1};

protected:

    // Well barcode.
    std::string well_barcode;

    // UMI barcode.
    std::string umi_barcode;

protected:

    // Set the group ID of FASTQ sequence using sequence information.
    virtual void setGroupId() override;

    // Set well barcode and UMI barcode.
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

    DGEIlluminaFASTQSequence();

    // parse_seq is defaulted to true for the parsing of IlluminaFASTQSequence.
    DGEIlluminaFASTQSequence(const FASTQSequenceLines& lines, bool parse_seq=true, bool parse_seq_id_level_1=false, bool parse_seq_id_level_2=false, bool flush_ostream=false);

    // parse_seq is defaulted to true for the parsing of IlluminaFASTQSequence.
    DGEIlluminaFASTQSequence(FASTQSequenceLines&& lines, bool parse_seq=true, bool parse_seq_id_level_1=false, bool parse_seq_id_level_2=false, bool flush_ostream=false);

    // parse_seq is defaulted to true for the parsing of IlluminaFASTQSequence.
    DGEIlluminaFASTQSequence(const std::string& line1, const std::string& line2, const std::string& line3, const std::string& line4, bool parse_seq=true, bool parse_seq_id_level_1=false, bool parse_seq_id_level_2=false, bool flush_ostream=false);

    // parse_seq is defaulted to true for the parsing of IlluminaFASTQSequence.
    DGEIlluminaFASTQSequence(std::string&& line1, std::string&& line2, std::string&& line3, std::string&& line4, bool parse_seq=true, bool parse_seq_id_level_1=false, bool parse_seq_id_level_2=false, bool flush_ostream=false);

    DGEIlluminaFASTQSequence(const IlluminaFASTQSequence& seq);

    DGEIlluminaFASTQSequence(IlluminaFASTQSequence&& seq);

    DGEIlluminaFASTQSequence(const FASTQSequence& seq, bool parse_seq_id_level_1=false, bool parse_seq_id_level_2=false);

    DGEIlluminaFASTQSequence(FASTQSequence&& seq, bool parse_seq_id_level_1=false, bool parse_seq_id_level_2=false);

    DGEIlluminaFASTQSequence(const DGEIlluminaFASTQSequence& seq);

    DGEIlluminaFASTQSequence(DGEIlluminaFASTQSequence&& seq);

    virtual ~DGEIlluminaFASTQSequence() noexcept;

    DGEIlluminaFASTQSequence& operator=(const DGEIlluminaFASTQSequence& seq);

    DGEIlluminaFASTQSequence& operator=(DGEIlluminaFASTQSequence&& seq);

    // Retrieve the well barcode of DGE FASTQ sequence.
    const std::string& getWellBarcode() const
    {
        return well_barcode;
    }

    // Retrieve the well barcode of DGE FASTQ sequence.
    std::string& getWellBarcode()
    {
        return well_barcode;
    }

    // Retrieve the UMI barcode of DGE FASTQ sequence.
    const std::string& getUMIBarcode() const
    {
        return umi_barcode;
    }

    // Retrieve the UMI barcode of DGE FASTQ sequence.
    std::string& getUMIBarcode()
    {
        return umi_barcode;
    }
};

using DGEIlluminaFASTQSequences = std::vector<DGEIlluminaFASTQSequence>;

}

#endif /* DGEIlluminaFASTQSequence_hpp */
