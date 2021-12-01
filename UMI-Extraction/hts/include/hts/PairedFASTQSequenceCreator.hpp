//
//  PairedFASTQSequenceCreator.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 8/2/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef PairedFASTQSequenceCreator_hpp
#define PairedFASTQSequenceCreator_hpp

#include <stdexcept>
#include <type_traits>
#include "FASTQSequence.hpp"

namespace hts
{

// Instantiated by FASTQSequence and its derived classes.
// Note: the class names used in is_same and is_base_of must be defined
// before the instantiation of PairedFASTQSequenceCreator, so the header files
// of FASTQSequence and IlluminaFASTQSequence classes need to be included
// before this file at the place of instantiation.

template<typename SeqType>
class PairedFASTQSequenceCreator
{
protected:

    // A pair of FAST sequences.
    SeqType seq_1, seq_2;

    // Group ID of FASTQ sequence.
    std::string group_id;

    // Check if two FASTQ sequence can be paired.
    bool parse_seq {false};

protected:

    // Set the group ID of FASTQ sequence using sequence information.
    void setGroupId()
    {
        group_id = seq_1.getGroupId();
    }

private:

    // Determine if two sequence objects of FASTQSequence or IlluminaFASTQSequence-derived type can be paired.
    void parse()
    {
        if constexpr (std::is_same_v<FASTQSequence, SeqType>)
        {
            // Check if the identifier line of two FASTQ sequences are the same.
            if(seq_1.getIdentifierLine() != seq_2.getIdentifierLine()) throw std::logic_error("Two FASTQ sequences cannot be paird: their Sequence Identifier lines are different");
        }
        else if constexpr (std::is_base_of_v<IlluminaFASTQSequence, SeqType>)
        {
            // Check if part 1 of Sequence Identifier is empty.
            if(seq_1.getSequenceIdentifierPart1().empty() || seq_2.getSequenceIdentifierPart1().empty()) throw std::logic_error("Two Illumina FASTQ sequences cannot be paird: the first part of Sequence Identifier line of one or both of two sequences are empty");
            // Check if part 1 of Sequence Identifier are different.
            if(seq_1.getSequenceIdentifierPart1() != seq_2.getSequenceIdentifierPart1()) throw std::logic_error("Two Illumina FASTQ sequences cannot be paird: the first part of the Sequence Identifier line of two sequences are different");
        }
        else throw std::logic_error("Cannot parse non-FASTQSequence pair");
    }

public:

    PairedFASTQSequenceCreator() {}

    PairedFASTQSequenceCreator(const SeqType& seq_1, const SeqType& seq_2, bool parse_seq=false) : seq_1{seq_1}, seq_2{seq_2}, parse_seq{parse_seq}
    {
        if constexpr (std::is_same_v<FASTQSequence,SeqType> || std::is_base_of_v<IlluminaFASTQSequence,SeqType>)
        {
            if(parse_seq) parse();
            setGroupId();
        }
        else throw std::logic_error("Cannot pair non-FASTQSequence objects");
    }

    PairedFASTQSequenceCreator(SeqType&& seq_1, SeqType&& seq_2, bool parse_seq=false) : seq_1{std::move(seq_1)}, seq_2{std::move(seq_2)}, parse_seq{parse_seq}
    {
        if constexpr (std::is_same_v<FASTQSequence,SeqType> || std::is_base_of_v<IlluminaFASTQSequence,SeqType>)
        {
            if(parse_seq) parse();
            setGroupId();
        }
        else throw std::logic_error("Cannot pair non-FASTQSequence objects");
    }

    PairedFASTQSequenceCreator(const PairedFASTQSequenceCreator& pair_seq) : seq_1{pair_seq.seq_1}, seq_2{pair_seq.seq_2}, parse_seq{pair_seq.parse_seq}
    {
        if constexpr (std::is_same_v<FASTQSequence,SeqType> || std::is_base_of_v<IlluminaFASTQSequence,SeqType>)
        {
            if(parse_seq) parse();
            setGroupId();
        }
        else throw std::logic_error("Cannot pair non-FASTQSequence objects");
    }

    PairedFASTQSequenceCreator(PairedFASTQSequenceCreator&& pair_seq) : seq_1{std::move(pair_seq.seq_1)}, seq_2{std::move(pair_seq.seq_2)}, parse_seq{pair_seq.parse_seq}
    {
        if constexpr (std::is_same_v<FASTQSequence,SeqType> || std::is_base_of_v<IlluminaFASTQSequence,SeqType>)
        {
            if(parse_seq) parse();
            setGroupId();
        }
        else throw std::logic_error("Cannot pair non-FASTQSequence objects");
    }

    virtual ~PairedFASTQSequenceCreator() noexcept {}

    PairedFASTQSequenceCreator& operator=(const PairedFASTQSequenceCreator& pair_seq)
    {
        if(this != &pair_seq)
        {
            seq_1 = pair_seq.seq_1;
            seq_2 = pair_seq.seq_2;
            group_id = pair_seq.group_id;
            parse_seq = pair_seq.parse_seq;
        }
        return *this;
    }

    PairedFASTQSequenceCreator& operator=(PairedFASTQSequenceCreator&& pair_seq)
    {
        if(this != &pair_seq)
        {
            seq_1 = std::move(pair_seq.seq_1);
            seq_2 = std::move(pair_seq.seq_2);
            group_id = std::move(pair_seq.group_id);
            parse_seq = pair_seq.parse_seq;
            pair_seq.parse_seq = false;
        }
        return *this;
    }

    const SeqType& getSequence1() const
    {
        return seq_1;
    }

    SeqType& getSequence1()
    {
        return seq_1;
    }

    const SeqType& getSequence2() const
    {
        return seq_2;
    }

    SeqType& getSequence2()
    {
        return seq_2;
    }

    const std::string& getGroupId() const
    {
        return group_id;
    }

    std::string& getGroupId()
    {
        return group_id;
    }

    bool getParseSequence() const
    {
        return parse_seq;
    }

    void setParseSequence(bool parse_seq_val)
    {
        parse_seq = parse_seq_val;
    }
};

/// Output of paired FASTQ sequence.
template<typename SeqType>
std::ostream& operator<<(std::ostream& os, const PairedFASTQSequenceCreator<SeqType>& seq)
{
    os << seq.getSequence1();
    os << seq.getSequence2();
    return os;
}

}

#endif /* PairedFASTQSequenceCreator_hpp */
