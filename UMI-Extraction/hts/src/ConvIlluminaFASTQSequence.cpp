//
//  ConvIlluminaFASTQSequence.cpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 5/4/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#include <hts/ConvIlluminaFASTQSequence.hpp>

namespace hts
{

ConvIlluminaFASTQSequence::ConvIlluminaFASTQSequence() {}

// parse_seq is defaulted to true for the parsing of IlluminaFASTQSequence.
ConvIlluminaFASTQSequence::ConvIlluminaFASTQSequence(const FASTQSequenceLines& lines, bool parse_seq, bool parse_seq_id_level_1, bool parse_seq_id_level_2, bool flush_ostream) : IlluminaFASTQSequence(lines, parse_seq, parse_seq_id_level_1, parse_seq_id_level_2, flush_ostream)
{
    setGroupId();
    setBarcode();
}

// parse_seq is defaulted to true for the parsing of IlluminaFASTQSequence.
ConvIlluminaFASTQSequence::ConvIlluminaFASTQSequence(FASTQSequenceLines&& lines, bool parse_seq, bool parse_seq_id_level_1, bool parse_seq_id_level_2, bool flush_ostream) : IlluminaFASTQSequence(std::move(lines), parse_seq, parse_seq_id_level_1, parse_seq_id_level_2, flush_ostream)
{
    setGroupId();
    setBarcode();
}

// parse_seq is defaulted to true for the parsing of IlluminaFASTQSequence.
ConvIlluminaFASTQSequence::ConvIlluminaFASTQSequence(const std::string& line1, const std::string& line2, const std::string& line3, const std::string& line4, bool parse_seq, bool parse_seq_id_level_1, bool parse_seq_id_level_2, bool flush_ostream) : IlluminaFASTQSequence(line1, line2, line3, line4, parse_seq, parse_seq_id_level_1, parse_seq_id_level_2, flush_ostream)
{
    setGroupId();
    setBarcode();
}

// parse_seq is defaulted to true for the parsing of IlluminaFASTQSequence.
ConvIlluminaFASTQSequence::ConvIlluminaFASTQSequence(std::string&& line1, std::string&& line2, std::string&& line3, std::string&& line4, bool parse_seq, bool parse_seq_id_level_1, bool parse_seq_id_level_2, bool flush_ostream) : IlluminaFASTQSequence(std::move(line1), std::move(line2), std::move(line3), std::move(line4), parse_seq, parse_seq_id_level_1, parse_seq_id_level_2, flush_ostream)
{
    setGroupId();
    setBarcode();
}

ConvIlluminaFASTQSequence::ConvIlluminaFASTQSequence(const IlluminaFASTQSequence& seq) : IlluminaFASTQSequence(seq)
{
    setGroupId();
    setBarcode();
}

ConvIlluminaFASTQSequence::ConvIlluminaFASTQSequence(IlluminaFASTQSequence&& seq) : IlluminaFASTQSequence(std::move(seq))
{
    setGroupId();
    setBarcode();
}

ConvIlluminaFASTQSequence::ConvIlluminaFASTQSequence(const FASTQSequence& seq, bool parse_seq_id_level_1, bool parse_seq_id_level_2) : IlluminaFASTQSequence(seq, parse_seq_id_level_1, parse_seq_id_level_2)
{
    setGroupId();
    setBarcode();
}

ConvIlluminaFASTQSequence::ConvIlluminaFASTQSequence(FASTQSequence&& seq, bool parse_seq_id_level_1, bool parse_seq_id_level_2) : IlluminaFASTQSequence(std::move(seq), parse_seq_id_level_1, parse_seq_id_level_2)
{
    setGroupId();
    setBarcode();
}

ConvIlluminaFASTQSequence::ConvIlluminaFASTQSequence(const ConvIlluminaFASTQSequence& seq) : IlluminaFASTQSequence(seq)
{
    setGroupId();
    setBarcode();
}

ConvIlluminaFASTQSequence::ConvIlluminaFASTQSequence(ConvIlluminaFASTQSequence&& seq) : IlluminaFASTQSequence(std::move(seq))
{
    setGroupId();
    setBarcode();
    seq.reset();
}

ConvIlluminaFASTQSequence::~ConvIlluminaFASTQSequence() noexcept {}

ConvIlluminaFASTQSequence& ConvIlluminaFASTQSequence::operator=(const ConvIlluminaFASTQSequence& seq)
{
    if(this != &seq)
    {
        IlluminaFASTQSequence::operator=(seq);
        well_barcode = seq.well_barcode;
    }
    return *this;
}

ConvIlluminaFASTQSequence& ConvIlluminaFASTQSequence::operator=(ConvIlluminaFASTQSequence&& seq)
{
    if(this != &seq)
    {
        IlluminaFASTQSequence::operator=(std::move(seq));
        well_barcode = std::move(seq.well_barcode);
        seq.reset();
    }
    return *this;
}

// Set the group ID of FASTQ sequence using sequence information.
void ConvIlluminaFASTQSequence::setGroupId()
{
    group_id = index_sequence;
}

void ConvIlluminaFASTQSequence::setBarcode()
{
    well_barcode = index_sequence;
}

void ConvIlluminaFASTQSequence::reset()
{
    well_barcode.clear();
}

}
