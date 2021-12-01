//
//  CompositedDGEIlluminaFASTQSequence.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 8/7/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef CompositedDGEIlluminaFASTQSequence_hpp
#define CompositedDGEIlluminaFASTQSequence_hpp

#include <vector>
#include "PairedDGEIlluminaFASTQSequence.hpp"

namespace hts
{

/// \brief The structure of a DGE version of Illumina FASTQ sequence.
/// Create a DGE version of Illumina FASTQ sequence combined from a pair of
/// standard Illumina FASTQ sequence. The difference between the DGE and the
/// standard Illumina FASTQ sequence includes:
///
/// 1) A field of 16-nt barcode (composed of 6-nt well barcode and 10-nt UMI
/// barcode) is appended to the end of the first part of the sequence ID line
/// of a standard Illumina FASTQ sequence.
/// 2) The part 2 of the sequence ID line of a standard Illumina FASTQ sequence
/// is removed.
///
/// The form of composite DGE Illumina FASTQ sequence is:
///
/// @HWI-D00704:48:C7302ANXX:1:1101:1103:2053:TAAGTACATAGCGTGG
/// GAGCTGCTATTTTATATTATGACTGCTTTTTAAGAAATTTTTGTTT
/// +
/// CCCBBGGGGGED1FFGG>GDFGEC@@;<:1:FGBDGG#########
class CompositedDGEIlluminaFASTQSequence : public FASTQSequence
{
public:

    // Separator for seq id
    static constexpr const std::size_t n_seq_id_parts = IlluminaFASTQSequence::n_seq_id_part_1_parts+1;

protected:

    /// The sequence ID of CompositedDGEIlluminaFASTQSequence is concatenated from
    /// the first part of standard Illumina sequence ID and a 16-nt barcode (6-nt
    /// well barcode, and 10-nt UMI barcode), in a form below:
    ///
    /// @HWI-D00704:48:C7302ANXX:1:1101:1103:2053:TAAGTACATAGCGTGG

    /// Seven standard elements of Illumina Sequence Identifier.
    std::string instrument_id;
    std::size_t run_number {0};
    std::string flowcell_id;
    std::size_t lane_number {0};
    std::size_t tile_number {0};
    std::size_t x_pos {0};
    std::size_t y_pos {0};

    /// Two barcode elements of from 3'-DGE sequencing data.

    /// 6-nt Well barcode.
    std::string well_barcode;

    /// 6-nt UMI barcode.
    std::string umi_barcode;

    /// Flag for validating composite DGE Illumina FASTQ sequence format.
    bool parse_compos_seq {false};

private:

    /// Initialize all data members of sequence attributes, including those in
    /// the base class FASTQSequence.
    /// Note:
    /// 1) Most sequence attributes are aleady included in the input argument
    ///    PairedDGEIlluminaFASTQSequence, so that they are just copied from
    ///    there
    /// 2) The 4-line buffer is initialized by initialize() directly
    ///    according to different input arguments.
    void initializeAttributes(const PairedDGEIlluminaFASTQSequence& paired_dge_seq, bool parse_seq_val=false, bool flush_ostream_val=false);

protected:

    /// \brief Initialize all data members.
    /// This function directly initializes the line buffer and then calls
    /// the initializeAttributes function to initialize other data members.
    /// Note: This function is used by the contructors that take the
    /// PairedDGEIlluminaFASTQSequence as its input argument, such that these
    /// constructors cannot use the base class FASTQSequence directly.
    void initialize(const PairedDGEIlluminaFASTQSequence& paired_dge_seq, bool parse_seq=false, bool flush_ostream=false);
    void initialize(PairedDGEIlluminaFASTQSequence&& paired_dge_seq, bool parse_seq=false, bool flush_ostream=false);

    // Set the group ID of FASTQ sequence using sequence information.
    virtual void setGroupId() override
    {
        group_id = well_barcode;
    }

    /// Parse the information specific to the DGE version of Illumina FASTQ
    /// sequence.
    /// Note: This function is used by the contructors that take the
    /// line buffer as its input argument, such that this function only needs
    /// to parse its own data members and use the base class FASTQSequence to
    /// parse the rest.
    void parse();

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

    CompositedDGEIlluminaFASTQSequence();

    /// Create CompositedDGEIlluminaFASTQSequence from PairedDGEIlluminaFASTQSequence.
    CompositedDGEIlluminaFASTQSequence(const PairedDGEIlluminaFASTQSequence& paired_dge_seq, bool parse_seq=false, bool flush_ostream=false);
    CompositedDGEIlluminaFASTQSequence(PairedDGEIlluminaFASTQSequence&& paired_dge_seq, bool parse_seq=false, bool flush_ostream=false);

    /// Create CompositedDGEIlluminaFASTQSequence from a pair of DGEIlluminaFASTQSequence.
    CompositedDGEIlluminaFASTQSequence(const DGEIlluminaFASTQSequence& seq_1, const DGEIlluminaFASTQSequence& seq_2, bool parse_paired_seq=false, bool parse_seq=false, bool flush_ostream=false);
    CompositedDGEIlluminaFASTQSequence(DGEIlluminaFASTQSequence&& seq_1, DGEIlluminaFASTQSequence&& seq_2, bool parse_paired_seq=false, bool parse_seq=false, bool flush_ostream=false);

    /// Create CompositedDGEIlluminaFASTQSequence from the four-line contents of
    /// existing composite DGE sequence.
    CompositedDGEIlluminaFASTQSequence(const std::string& line1, const std::string& line2, const std::string& line3, const std::string& line4, bool parse_compos_seq=false, bool parse_seq=false, bool flush_ostream=false);
    CompositedDGEIlluminaFASTQSequence(std::string&& line1, std::string&& line2, std::string&& line3, std::string&& line4, bool parse_compos_seq=false, bool parse_seq=false, bool flush_ostream=false);
    CompositedDGEIlluminaFASTQSequence(const FASTQSequenceLines& lines, bool parse_compos_seq=false, bool parse_seq=false, bool flush_ostream=false);
    CompositedDGEIlluminaFASTQSequence(FASTQSequenceLines&& lines, bool parse_compos_seq=false, bool parse_seq=false, bool flush_ostream=false);

    CompositedDGEIlluminaFASTQSequence(const CompositedDGEIlluminaFASTQSequence& seq);

    CompositedDGEIlluminaFASTQSequence(CompositedDGEIlluminaFASTQSequence&& seq);

    virtual ~CompositedDGEIlluminaFASTQSequence() noexcept;

    CompositedDGEIlluminaFASTQSequence& operator=(const CompositedDGEIlluminaFASTQSequence& seq);

    CompositedDGEIlluminaFASTQSequence& operator=(CompositedDGEIlluminaFASTQSequence&& seq);

    const std::string& getInstrumentId() const
    {
        return instrument_id;
    }

    std::string& getInstrumentId()
    {
        return instrument_id;
    }

    std::size_t getRunNumber() const
    {
        return run_number;
    }

    const std::string& getFlowcellId() const
    {
        return flowcell_id;
    }

    std::string& getFlowcellId()
    {
        return flowcell_id;
    }

    std::size_t getLaneNumber() const
    {
        return lane_number;
    }

    std::size_t getTileNumber() const
    {
        return tile_number;
    }

    std::size_t getXPos() const
    {
        return x_pos;
    }

    std::size_t getYPos() const
    {
        return y_pos;
    }

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

using CompositedDGEIlluminaFASTQSequences = std::vector<CompositedDGEIlluminaFASTQSequence>;

}

#endif /* CompositedDGEIlluminaFASTQSequence_hpp */
