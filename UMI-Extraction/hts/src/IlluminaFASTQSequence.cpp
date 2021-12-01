//
//  IlluminaFASTQSequence.cpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 5/4/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#include <vector>
#include <utility>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <utk/StringUtils.hpp>
#include <hts/IlluminaFASTQSequence.hpp>

namespace hts
{

IlluminaFASTQSequence::IlluminaFASTQSequence() : FASTQSequence() {}

// parse_seq is defaulted to true for the parsing of IlluminaFASTQSequence.
IlluminaFASTQSequence::IlluminaFASTQSequence(const FASTQSequenceLines& lines, bool parse_seq, bool parse_seq_id_level_1, bool parse_seq_id_level_2, bool flush_ostream) : FASTQSequence(lines, parse_seq, flush_ostream), parse_seq_id_level_1{parse_seq_id_level_1}, parse_seq_id_level_2{parse_seq_id_level_2}
{
    // Adjust IlluminaFASTQSequence parsing options.
    adjustParseOption();
    // No need to parse FASTQSequence information because FASTQSequence
    // constructor is already called by IlluminaFASTQSequence constructor.
    if(parse_seq)
    {
        parse();
        setGroupId();
    }
}

// parse_seq is defaulted to true for the parsing of IlluminaFASTQSequence.
IlluminaFASTQSequence::IlluminaFASTQSequence(FASTQSequenceLines&& lines, bool parse_seq, bool parse_seq_id_level_1, bool parse_seq_id_level_2, bool flush_ostream) : FASTQSequence(std::move(lines), parse_seq, flush_ostream), parse_seq_id_level_1{parse_seq_id_level_1}, parse_seq_id_level_2{parse_seq_id_level_2}
{
    // Adjust IlluminaFASTQSequence parsing options.
    adjustParseOption();
    // No need to parse FASTQSequence information because FASTQSequence
    // constructor is already called by IlluminaFASTQSequence constructor.
    if(parse_seq)
    {
        parse();
        setGroupId();
    }
}

// parse_seq is defaulted to true for the parsing of IlluminaFASTQSequence.
IlluminaFASTQSequence::IlluminaFASTQSequence(const std::string& line1, const std::string& line2, const std::string& line3, const std::string& line4, bool parse_seq, bool parse_seq_id_level_1, bool parse_seq_id_level_2, bool flush_ostream) : FASTQSequence(line1, line2, line3, line4, parse_seq, flush_ostream), parse_seq_id_level_1{parse_seq_id_level_1}, parse_seq_id_level_2{parse_seq_id_level_2}
{
    // Adjust IlluminaFASTQSequence parsing options.
    adjustParseOption();
    // No need to parse FASTQSequence information because FASTQSequence
    // constructor is already called by IlluminaFASTQSequence constructor.
    if(parse_seq)
    {
        parse();
        setGroupId();
    }
}

// parse_seq is defaulted to true for the parsing of IlluminaFASTQSequence.
IlluminaFASTQSequence::IlluminaFASTQSequence(std::string&& line1, std::string&& line2, std::string&& line3, std::string&& line4, bool parse_seq, bool parse_seq_id_level_1, bool parse_seq_id_level_2, bool flush_ostream) : FASTQSequence(std::move(line1), std::move(line2), std::move(line3), std::move(line4), parse_seq, flush_ostream), parse_seq_id_level_1{parse_seq_id_level_1}, parse_seq_id_level_2{parse_seq_id_level_2}
{
    // Adjust IlluminaFASTQSequence parsing options.
    adjustParseOption();
    // No need to parse FASTQSequence information because FASTQSequence
    // constructor is already called by IlluminaFASTQSequence constructor.
    if(parse_seq)
    {
        parse();
        setGroupId();
    }
}

IlluminaFASTQSequence::IlluminaFASTQSequence(const FASTQSequence& seq, bool parse_seq_id_level_1, bool parse_seq_id_level_2) : FASTQSequence(seq), parse_seq_id_level_1{parse_seq_id_level_1}, parse_seq_id_level_2{parse_seq_id_level_2}
{
    // Adjust IlluminaFASTQSequence parsing options.
    adjustParseOption();
    // No need to parse FASTQSequence information because FASTQSequence
    // constructor is already called by IlluminaFASTQSequence constructor.
    if(parse_seq)
    {
        parse();
        setGroupId();
    }
}

IlluminaFASTQSequence::IlluminaFASTQSequence(FASTQSequence&& seq, bool parse_seq_id_level_1, bool parse_seq_id_level_2) : FASTQSequence(std::move(seq)), parse_seq_id_level_1{parse_seq_id_level_1}, parse_seq_id_level_2{parse_seq_id_level_2}
{
    // Adjust IlluminaFASTQSequence parsing options.
    adjustParseOption();
    // No need to parse FASTQSequence information because FASTQSequence
    // constructor is already called by IlluminaFASTQSequence constructor.
    if(parse_seq)
    {
        parse();
        setGroupId();
    }
}

IlluminaFASTQSequence::IlluminaFASTQSequence(const IlluminaFASTQSequence& seq) : FASTQSequence(seq), seq_id_part_1{seq.seq_id_part_1}, seq_id_part_2{seq.seq_id_part_2}, instrument_id{seq.instrument_id}, run_number{seq.run_number}, flowcell_id{seq.flowcell_id}, lane_number{seq.lane_number}, tile_number{seq.tile_number}, x_pos{seq.x_pos}, y_pos{seq.y_pos}, read_number{seq.read_number}, is_filtered{seq.is_filtered}, control_number{seq.control_number}, index_sequence{seq.index_sequence}, parse_seq_id_level_1{seq.parse_seq_id_level_1}, parse_seq_id_level_2{seq.parse_seq_id_level_2} {}

IlluminaFASTQSequence::IlluminaFASTQSequence(IlluminaFASTQSequence&& seq) : FASTQSequence(std::move(seq)), seq_id_part_1{std::move(seq.seq_id_part_1)}, seq_id_part_2{std::move(seq.seq_id_part_2)}, instrument_id{std::move(seq.instrument_id)}, run_number{seq.run_number}, flowcell_id{std::move(seq.flowcell_id)}, lane_number{seq.lane_number}, tile_number{seq.tile_number}, x_pos{seq.x_pos}, y_pos{seq.y_pos}, read_number{seq.read_number}, is_filtered{seq.is_filtered}, control_number{seq.control_number}, index_sequence{std::move(seq.index_sequence)}, parse_seq_id_level_1{seq.parse_seq_id_level_1}, parse_seq_id_level_2{seq.parse_seq_id_level_2}
{
    seq.reset();
}

IlluminaFASTQSequence::~IlluminaFASTQSequence() noexcept {}

IlluminaFASTQSequence& IlluminaFASTQSequence::operator=(const IlluminaFASTQSequence& seq)
{
    if(this != &seq)
    {
        FASTQSequence::operator=(seq);
        seq_id_part_1 = seq.seq_id_part_1;
        seq_id_part_2 = seq.seq_id_part_2;
        instrument_id = seq.instrument_id;
        run_number = seq.run_number;
        flowcell_id = seq.flowcell_id;
        lane_number = seq.lane_number;
        tile_number = seq.tile_number;
        x_pos = seq.x_pos;
        y_pos = seq.y_pos;
        read_number = seq.read_number;
        is_filtered = seq.is_filtered;
        control_number = seq.control_number;
        index_sequence = seq.index_sequence;
        parse_seq_id_level_1 = seq.parse_seq_id_level_1;
        parse_seq_id_level_2 = seq.parse_seq_id_level_2;
    }
    return *this;
}

IlluminaFASTQSequence& IlluminaFASTQSequence::operator=(IlluminaFASTQSequence&& seq)
{
    if(this != &seq)
    {
        FASTQSequence::operator=(std::move(seq));
        seq_id_part_1 = std::move(seq.seq_id_part_1);
        seq_id_part_2 = std::move(seq.seq_id_part_2);
        instrument_id = std::move(seq.instrument_id);
        run_number = seq.run_number;
        flowcell_id = std::move(seq.flowcell_id);
        lane_number = seq.lane_number;
        tile_number = seq.tile_number;
        x_pos = seq.x_pos;
        y_pos = seq.y_pos;
        read_number = seq.read_number;
        is_filtered = seq.is_filtered;
        control_number = seq.control_number;
        index_sequence = std::move(seq.index_sequence);
        parse_seq_id_level_1 = seq.parse_seq_id_level_1;
        parse_seq_id_level_2 = seq.parse_seq_id_level_2;
        seq.reset();
    }
    return *this;
}

void IlluminaFASTQSequence::reset()
{
    seq_id_part_1.clear();
    seq_id_part_2.clear();
    instrument_id.clear();
    run_number = 0;
    flowcell_id.clear();
    lane_number = 0;
    tile_number = 0;
    x_pos = 0;
    y_pos = 0;
    read_number = 0;
    is_filtered = 'N';
    control_number = 0;
    index_sequence.clear();
    parse_seq_id_level_1 = false;
    parse_seq_id_level_2 = false;
}

// Parse the information specific to Illumina FASTQ sequence.
void IlluminaFASTQSequence::parse()
{
    // Parse the Sequence ID line of Illumina FASTQ sequence.
    if(parse_seq_id_level_1)
    {
        // Parse Sequence ID line at level 1: two parts of Sequence ID line.
        const auto& seq_id_line = lines[SequenceIdentifier];
        
        // Extract two parts of Sequence Identifier line.
        auto seq_id_parts = utk::splitString(seq_id_line, space_sep);
        if(seq_id_parts.size() != 2) throw std::logic_error("Sequence Identifier line doesn't contain exactly two parts separated by a space");
        seq_id_part_1 = seq_id_parts.front();
        seq_id_part_2 = seq_id_parts.back();
        
        // Parse Sequence ID line at level 2: each fields of both parts of Sequence ID line.
        if(parse_seq_id_level_2)
        {
            // Parse the first part of Sequence Identifier line.
            if(auto seq_id_part_1_parts = utk::splitString(seq_id_part_1, colon_sep); seq_id_part_1_parts.size() == n_seq_id_part_1_parts)
            {
                auto it = seq_id_part_1_parts.cbegin();
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
            }
            else
            {
                std::ostringstream err_msg;
                err_msg << "The first part of Sequence Identifier line must have " << n_seq_id_part_1_parts << " elements!";
                throw std::logic_error(err_msg.str());
            }
            
            // Parse the second part of Sequence Identifier line.
            if(auto seq_id_part_2_parts = utk::splitString(seq_id_part_2, colon_sep); seq_id_part_2_parts.size() == n_seq_id_part_2_parts)
            {
                auto it = seq_id_part_2_parts.cbegin();
                try {
                    read_number = std::stoul(*(it++));
                } catch(const std::logic_error& e) {
                    throw std::logic_error("Failed to convert read number to unsigned long type");
                }
                is_filtered = (it++)->c_str()[0];
                try {
                    control_number = std::stoul(*(it++));
                } catch(const std::logic_error& e) {
                    throw std::logic_error("Failed to convert control number to unsigned long type");
                }
                index_sequence = *(it++);
            }
            else
            {
                std::ostringstream err_msg;
                err_msg << "The second part of Sequence Identifier line must have " << n_seq_id_part_2_parts << " elements!";
                throw std::logic_error(err_msg.str());
            }
        }
    }
}

}
