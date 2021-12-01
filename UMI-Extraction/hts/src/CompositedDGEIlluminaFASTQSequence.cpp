//
//  CompositedDGEIlluminaFASTQSequence.cpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 8/7/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#include <utk/StringUtils.hpp>
#include <hts/CompositedDGEIlluminaFASTQSequence.hpp>
#include <iostream>

namespace hts
{

CompositedDGEIlluminaFASTQSequence::CompositedDGEIlluminaFASTQSequence() {}

CompositedDGEIlluminaFASTQSequence::CompositedDGEIlluminaFASTQSequence(const PairedDGEIlluminaFASTQSequence& paired_dge_seq, bool parse_seq, bool flush_ostream)
{
    initialize(paired_dge_seq, parse_seq, flush_ostream);
}

CompositedDGEIlluminaFASTQSequence::CompositedDGEIlluminaFASTQSequence(PairedDGEIlluminaFASTQSequence&& paired_dge_seq, bool parse_seq, bool flush_ostream)
{
    initialize(std::move(paired_dge_seq), parse_seq, flush_ostream);
}

CompositedDGEIlluminaFASTQSequence::CompositedDGEIlluminaFASTQSequence(const DGEIlluminaFASTQSequence& seq_1, const DGEIlluminaFASTQSequence& seq_2, bool parse_paired_seq, bool parse_seq, bool flush_ostream)
{
    initialize(PairedDGEIlluminaFASTQSequence(seq_1, seq_2, parse_paired_seq), parse_seq, flush_ostream);
}

CompositedDGEIlluminaFASTQSequence::CompositedDGEIlluminaFASTQSequence(DGEIlluminaFASTQSequence&& seq_1, DGEIlluminaFASTQSequence&& seq_2, bool parse_paired_seq, bool parse_seq, bool flush_ostream)
{
    initialize(PairedDGEIlluminaFASTQSequence(std::move(seq_1), std::move(seq_2), parse_paired_seq), parse_seq, flush_ostream);

}

CompositedDGEIlluminaFASTQSequence::CompositedDGEIlluminaFASTQSequence(const std::string& line1, const std::string& line2, const std::string& line3, const std::string& line4, bool parse_compos_seq, bool parse_seq, bool flush_ostream) : FASTQSequence(line1, line2, line3, line4, parse_seq, flush_ostream), parse_compos_seq{parse_compos_seq}
{
    if(parse_compos_seq) parse();
}

CompositedDGEIlluminaFASTQSequence::CompositedDGEIlluminaFASTQSequence(std::string&& line1, std::string&& line2, std::string&& line3, std::string&& line4, bool parse_compos_seq, bool parse_seq, bool flush_ostream) : FASTQSequence(std::move(line1), std::move(line2), std::move(line3), std::move(line4), parse_seq, flush_ostream), parse_compos_seq{parse_compos_seq}
{
    if(parse_compos_seq) parse();
}

CompositedDGEIlluminaFASTQSequence::CompositedDGEIlluminaFASTQSequence(const FASTQSequenceLines& lines, bool parse_compos_seq, bool parse_seq, bool flush_ostream) : FASTQSequence(lines, parse_seq, flush_ostream), parse_compos_seq{parse_compos_seq}
{
    if(parse_compos_seq) parse();
}

CompositedDGEIlluminaFASTQSequence::CompositedDGEIlluminaFASTQSequence(FASTQSequenceLines&& lines, bool parse_compos_seq, bool parse_seq, bool flush_ostream) : FASTQSequence(std::move(lines), parse_seq, flush_ostream), parse_compos_seq{parse_compos_seq}
{
    if(parse_compos_seq) parse();
}

CompositedDGEIlluminaFASTQSequence::CompositedDGEIlluminaFASTQSequence(const CompositedDGEIlluminaFASTQSequence& seq) : FASTQSequence(seq), instrument_id{seq.instrument_id}, run_number{seq.run_number}, flowcell_id{seq.flowcell_id}, lane_number{seq.lane_number}, tile_number{seq.tile_number}, x_pos{seq.x_pos}, y_pos{seq.y_pos}, well_barcode{seq.well_barcode}, umi_barcode{seq.umi_barcode}, parse_compos_seq{seq.parse_compos_seq} {}

CompositedDGEIlluminaFASTQSequence::CompositedDGEIlluminaFASTQSequence(CompositedDGEIlluminaFASTQSequence&& seq) : FASTQSequence(std::move(seq)), instrument_id{std::move(seq.instrument_id)}, run_number{seq.run_number}, flowcell_id{std::move(seq.flowcell_id)}, lane_number{seq.lane_number}, tile_number{seq.tile_number}, x_pos{seq.x_pos}, y_pos{seq.y_pos}, well_barcode{std::move(seq.well_barcode)}, umi_barcode{std::move(seq.umi_barcode)}, parse_compos_seq{seq.parse_compos_seq}
{
    // Clear all data members of seq.
    seq.reset();
}

CompositedDGEIlluminaFASTQSequence::~CompositedDGEIlluminaFASTQSequence() noexcept {}

CompositedDGEIlluminaFASTQSequence& CompositedDGEIlluminaFASTQSequence::operator=(const CompositedDGEIlluminaFASTQSequence& seq)
{
    if(this != &seq)
    {
        FASTQSequence::operator=(seq);
        instrument_id = seq.instrument_id;
        run_number = seq.run_number;
        flowcell_id = seq.flowcell_id;
        lane_number = seq.lane_number;
        tile_number = seq.tile_number;
        x_pos = seq.x_pos;
        y_pos = seq.y_pos;
        well_barcode = seq.well_barcode;
        umi_barcode = seq.umi_barcode;
        flush_ostream = seq.flush_ostream;
        parse_compos_seq = seq.parse_compos_seq;
    }
    return *this;
}

CompositedDGEIlluminaFASTQSequence& CompositedDGEIlluminaFASTQSequence::operator=(CompositedDGEIlluminaFASTQSequence&& seq)
{
    if(this != &seq)
    {
        FASTQSequence::operator=(seq);
        instrument_id = std::move(seq.instrument_id);
        run_number = seq.run_number;
        flowcell_id = std::move(seq.flowcell_id);
        lane_number = seq.lane_number;
        tile_number = seq.tile_number;
        x_pos = seq.x_pos;
        y_pos = seq.y_pos;
        well_barcode = std::move(seq.well_barcode);
        umi_barcode = std::move(seq.umi_barcode);
        flush_ostream = seq.flush_ostream;
        parse_compos_seq = seq.parse_compos_seq;
        // Clear all data members of seq.
        seq.reset();
    }
    return *this;
}

void CompositedDGEIlluminaFASTQSequence::initialize(const PairedDGEIlluminaFASTQSequence& paired_dge_seq, bool parse_seq, bool flush_ostream)
{
    // Fill in FASTQSequenceLines.
    const DGEIlluminaFASTQSequence& dge_seq1 = paired_dge_seq.getSequence1();
    const DGEIlluminaFASTQSequence& dge_seq2 = paired_dge_seq.getSequence2();
    // The Identifier line is concatenated from the first part of
    // sequence identifier, the well barcode, and the UMI barcode
    // of the first read of DGE sequence pair.
    lines[0] = dge_seq1.getSequenceIdentifierPart1() + ":" + dge_seq1.getWellBarcode() + dge_seq1.getUMIBarcode();
    // The Sequence line is the that of the second read of DGE
    // sequence pair.
    lines[1] = dge_seq2.getSequenceLine();
    lines[2] = "+";
    // The Quality line is the that of the second read of DGE sequence
    // pair.
    lines[3] = dge_seq2.getQualityLine();

    // Initialize attribute data members.
    initializeAttributes(paired_dge_seq, parse_seq, flush_ostream);
}

void CompositedDGEIlluminaFASTQSequence::initialize(PairedDGEIlluminaFASTQSequence&& paired_dge_seq, bool parse_seq, bool flush_ostream)
{
    DGEIlluminaFASTQSequence& dge_seq1 = paired_dge_seq.getSequence1();
    DGEIlluminaFASTQSequence& dge_seq2 = paired_dge_seq.getSequence2();
    // The Identifier line is concatenated from the first part of
    // sequence identifier, the well barcode, and the UMI barcode
    // of the first read of DGE sequence pair.
    lines[0] = std::move(dge_seq1.getSequenceIdentifierPart1()) + ":" + std::move(dge_seq1.getWellBarcode()) + std::move(dge_seq1.getUMIBarcode());
    // The Sequence line is the that of the second read of DGE
    // sequence pair.
    lines[1] = std::move(dge_seq2.getSequenceLine());
    lines[2] = "+";
    // The Quality line is the that of the second read of DGE sequence
    // pair.
    lines[3] = std::move(dge_seq2.getQualityLine());

    // Initialize attribute data members.
    initializeAttributes(paired_dge_seq, parse_seq, flush_ostream);
}

void CompositedDGEIlluminaFASTQSequence::initializeAttributes(const PairedDGEIlluminaFASTQSequence& paired_dge_seq, bool parse_seq_val, bool flush_ostream_val)
{
    // Fill in FASTQSequenceLines.
    const DGEIlluminaFASTQSequence& dge_seq1 = paired_dge_seq.getSequence1();
    const DGEIlluminaFASTQSequence& dge_seq2 = paired_dge_seq.getSequence2();

    // Set the flag of flush output stream.
    flush_ostream = flush_ostream_val;

    // Obtain read length.
    read_length = dge_seq2.getReadLength();

    // Parse the composite FASTQ sequence.
    parse_seq = parse_seq_val;
    if(parse_seq) FASTQSequence::parse();

    // Fill in sequence Identifier info.
    instrument_id = dge_seq1.getInstrumentId();
    run_number = dge_seq1.getRunNumber();
    flowcell_id = dge_seq1.getFlowcellId();
    lane_number = dge_seq1.getLaneNumber();
    tile_number = dge_seq1.getTileNumber();
    x_pos = dge_seq1.getXPos();
    y_pos = dge_seq1.getYPos();
    // Set well and UMI barcodes using the read 1 of paired DGE FASTQ sequence.
    well_barcode = dge_seq1.getWellBarcode();
    umi_barcode = dge_seq1.getUMIBarcode();

    // Fill in group ID.
    // Note: this function can be invoked only after well_barcode is assigned
    // with some value.
    setGroupId();
}

// Clear all data members.
void CompositedDGEIlluminaFASTQSequence::reset()
{
    instrument_id.clear();
    run_number = 0;
    flowcell_id.clear();
    lane_number = 0;
    tile_number = 0;
    x_pos = 0;
    y_pos = 0;
    well_barcode.clear();
    umi_barcode.clear();
    flush_ostream = false;
}

// Parse the information specific to the DGE version of Illumina FASTQ
// sequence.
void CompositedDGEIlluminaFASTQSequence::parse()
{
    // Parse the Sequence Identifier line of composite DGE Illumina FASTQ sequence.
    // The sequence ID of CompositedDGEIlluminaFASTQSequence is concatenated from
    // the first part of standard Illumina sequence ID and a 16-nt barcode (6-nt
    // well barcode, and 10-nt UMI barcode).
    if(auto parts = utk::splitString(lines[IlluminaFASTQSequence::SequenceIdentifier], IlluminaFASTQSequence::colon_sep); parts.size() == n_seq_id_parts)
    {
        auto it = parts.cbegin();
        instrument_id = *(it++);
        // Remove '@'
        if(instrument_id.front() == FASTQSequence::id_line_beg_char) instrument_id = instrument_id.substr(1);
        try
        {
            run_number = std::stoul(*(it++));
        }
        catch(const std::logic_error& e)
        {
            throw std::logic_error("Failed to convert run number to unsigned long type");
        }
        flowcell_id = *(it++);
        try
        {
            lane_number = std::stoul(*(it++));
        }
        catch(const std::logic_error& e)
        {
            throw std::logic_error("Failed to convert lane number to unsigned long type");
        }
        try
        {
            tile_number = std::stoul(*(it++));
        }
        catch(const std::logic_error& e)
        {
            throw std::logic_error("Failed to convert tile number to unsigned long type");
        }
        try
        {
            x_pos = std::stoul(*(it++));
        }
        catch(const std::logic_error& e)
        {
            throw std::logic_error("Failed to convert X position to unsigned long type");
        }
        try
        {
            y_pos = std::stoul(*(it++));
        }
        catch(const std::logic_error& e)
        {
            throw std::logic_error("Failed to convert Y position to unsigned long type");
        }
        // Set well barcode and UMI barcode.
        const std::string& barcode = *(it++);
        if(barcode.length() != DGEIlluminaFASTQSequence::well_barcode_length+DGEIlluminaFASTQSequence::umi_barcode_length) throw std::logic_error("The length of barcode part of SeqId line of composited DGE Illumina FASTQ sequence must be the sum of the lengths of well and UMI barcodes!");
        well_barcode = barcode.substr(DGEIlluminaFASTQSequence::well_barcode_beg_pos, DGEIlluminaFASTQSequence::well_barcode_length);
        umi_barcode = barcode.substr(DGEIlluminaFASTQSequence::umi_barcode_beg_pos, DGEIlluminaFASTQSequence::umi_barcode_length);
    }
    else
    {
        std::ostringstream err_msg;
        err_msg << "Sequence Identifier line must have " << n_seq_id_parts << " elements!";
        throw std::logic_error(err_msg.str());
    }

    // Fill in group ID.
    setGroupId();
}

}
