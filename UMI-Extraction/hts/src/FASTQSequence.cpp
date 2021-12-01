//
//  FASTQSequence.cpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 5/2/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#include <stdexcept>
#include <hts/FASTQSequence.hpp>

namespace hts
{

FASTQSequence::FASTQSequence() {}

    FASTQSequence::FASTQSequence(const FASTQSequenceLines& lines, bool parse_seq, bool flush_ostream) : lines{lines}, parse_seq{parse_seq}, flush_ostream{flush_ostream}
{
    if(parse_seq) parse();
}

FASTQSequence::FASTQSequence(FASTQSequenceLines&& lines, bool parse_seq, bool flush_ostream) : lines{std::move(lines)}, parse_seq{parse_seq}, flush_ostream{flush_ostream}
{
    if(parse_seq) parse();
}

FASTQSequence::FASTQSequence(const std::string& line1, const std::string& line2, const std::string& line3, const std::string& line4, bool parse_seq, bool flush_ostream) : lines{{line1, line2, line3, line4}}, parse_seq{parse_seq}, flush_ostream{flush_ostream}
{
    if(parse_seq) parse();
}

FASTQSequence::FASTQSequence(std::string&& line1, std::string&& line2, std::string&& line3, std::string&& line4, bool parse_seq, bool flush_ostream) : lines{{std::move(line1), std::move(line2), std::move(line3), std::move(line4)}}, parse_seq{parse_seq}, flush_ostream{flush_ostream}
{
    if(parse_seq) parse();
}

FASTQSequence::FASTQSequence(const FASTQSequence& seq) : lines{seq.lines}, read_length{seq.read_length}, parse_seq{seq.parse_seq}, flush_ostream{seq.flush_ostream} {}

FASTQSequence::FASTQSequence(FASTQSequence&& seq) : lines{std::move(seq.lines)}, read_length{seq.read_length}, parse_seq{seq.parse_seq}, flush_ostream{seq.flush_ostream}
{
    seq.reset();
}

FASTQSequence::~FASTQSequence() noexcept {}

FASTQSequence& FASTQSequence::operator=(const FASTQSequence& seq)
{
    if(this != &seq)
    {
        lines = seq.lines;
        read_length = seq.read_length;
        group_id = seq.group_id;
        parse_seq = seq.parse_seq;
        flush_ostream = seq.flush_ostream;
    }
    return *this;
}

FASTQSequence& FASTQSequence::operator=(FASTQSequence&& seq)
{
    if(this != &seq)
    {
        lines = std::move(seq.lines);
        read_length = seq.read_length;
        group_id = std::move(seq.group_id);
        parse_seq = seq.parse_seq;
        flush_ostream = seq.flush_ostream;
        seq.reset();
    }
    return *this;
}

bool FASTQSequence::empty() const
{
    bool result = true;
    for(const auto& line : lines)
    {
        if(!line.empty())
        {
            result = false;
            break;
        }
    }
    return result;
}

/// Clear all data members.
void FASTQSequence::reset()
{
    for(auto& line : lines) line.clear();
    read_length = 0;
    group_id.clear();
    parse_seq = false;
    flush_ostream = false;
}

// Parse the information specific to Illumina FASTQ sequence.
void FASTQSequence::parse()
{
    // Identifier line must start with @
    if(lines[Identifier].front() != '@') throw std::logic_error("Identifier line doesn't start with @");
    
    // Option line must start with +
    if(lines[Option].front() != '+') throw std::logic_error("Option line doesn't start with +");
    
    // Sequence line and Quality line must have an equal read length.
    auto seq_length = lines[Sequence].length();
    auto qual_length = lines[Quality].length();
    if(seq_length == qual_length) read_length = seq_length;
    else throw std::logic_error("The lengths of sequence line and quality line are not equal");
    read_length = seq_length;
}

// Output of FASTQ sequence.
std::ostream& operator<<(std::ostream& os, const FASTQSequence& seq)
{
    os << seq[FASTQSequence::Identifier] << '\n' << seq[FASTQSequence::Sequence] << '\n' << seq[FASTQSequence::Option] << '\n' << seq[FASTQSequence::Quality];
    if(seq.getFlushOstream()) os.flush();
    return os;
}

}
