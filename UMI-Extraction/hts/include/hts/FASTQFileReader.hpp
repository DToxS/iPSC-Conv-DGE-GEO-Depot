//
//  FASTQFileReader.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 8/30/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef FASTQFileReader_hpp
#define FASTQFileReader_hpp

#include <tuple>
#include <utility>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <utk/LineReader.hpp>
#include "FASTQSequence.hpp"

namespace hts
{

/// \brief A reader to retrieve each sequence from FASTQ file
/// This class reads in each sequence fragment from a FASTQ file and creates
/// corresponding FASTQSequence object. Batched reading is also supported.
template<typename SeqType, typename... ArgTypes>
class FASTQFileReader : public utk::LineReader
{
public:

    using SeqTypes = std::vector<SeqType>;
    using TupleType = std::tuple<ArgTypes...>;

private:

    /// \brief Flag for a sequence fragment been read in
    bool seq_read {false};

    /// \brief Arguments for creating a FASTQ seqiemce
    TupleType seq_args;

private:

    template<typename FASTQSequenceLinesType, std::size_t... Indexes>
    SeqType makeSequence_Impl(FASTQSequenceLinesType&& lines, std::index_sequence<Indexes...>)
    {
        return SeqType(std::forward<FASTQSequenceLinesType>(lines), std::get<Indexes>(seq_args)...);
    }

protected:

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
    void reset()
    {
        // Reset sequence-read flag to false.
        seq_read = false;
        // Don't clear seq_args, as it's an input argument.
    }

public:

    FASTQFileReader() = delete;

    explicit FASTQFileReader(const std::string& file_name, const std::string& line_delim_type="unix", const ArgTypes&... args) : utk::LineReader(file_name, line_delim_type), seq_args{std::make_tuple(args...)} {}

    template<typename FASTQSequenceLinesType>
    SeqType makeSequence(FASTQSequenceLinesType&& lines)
    {
        constexpr std::size_t n_args = std::tuple_size<TupleType>::value;
        return makeSequence_Impl(std::forward<FASTQSequenceLinesType>(lines), std::make_index_sequence<n_args>());
    }

    /// Forbid copy construction behavior.
    FASTQFileReader(const FASTQFileReader& file) = delete;

    /// Allow move construction behavior.
    FASTQFileReader(FASTQFileReader&& file) :
        utk::LineReader(std::move(file)),
        seq_read{file.seq_read},
        seq_args{std::move(file.seq_args)} {}

    virtual ~FASTQFileReader() noexcept {}

    /// Forbid copy assignment behavior.
    FASTQFileReader& operator=(const FASTQFileReader& file) = delete;

    /// Allow copy assignment behavior.
    FASTQFileReader& operator=(FASTQFileReader&& file)
    {
        if(this != &file)
        {
            utk::LineReader::operator=(std::move(file));
            seq_read = file.seq_read;
            seq_args = std::move(file.seq_args);
        }
        return *this;
    }

    bool isSequenceRead() const
    {
        return seq_read;
    }

    const TupleType& getArguments() const
    {
        return seq_args;
    }

    // Read in a FASTQ sequence.
    //
    // After each call to readSequence, use two rules to determine
    // current reading status:
    // 1) Call isFileEnd to determine if the end of FASTQ file is reached.
    // 2) Call isSequenceRead to determine if a FASTQ sequence has been
    //    read in successfully.
    // Note: all ill-formed FASTQ sequences are discarded.
    SeqType readSequence()
    {
        seq_read = false;
        SeqType seq;

        if(!isFileEnd())
        {
            // Read in 4 lines from FASTQ sequence file.
            FASTQSequenceLines seq_lines;
            auto lines = readLines(FASTQSequence::n_fastq_sequence_lines);
            std::size_t counts = 0;
            for(auto& line : lines) seq_lines[counts++] = std::move(line);

            // Create a FASTQSequence-derived object from the 4 lines read.
            if(counts == FASTQSequence::n_fastq_sequence_lines)
            {
                try
                {
                    // The temporary FASTQSequence is moved to seq.
                    seq = makeSequence(std::move(seq_lines));
                    seq_read = true;
                } catch(const std::logic_error& e)
                {
                    // Error occurs in parsing FASTQ sequence.
                    std::cerr << "Warning: " << e.what() << '!' << std::endl;
                }
            }
        }

        return seq;
    }

    // An efficient version of readSequence that avoids a copy operation.
    // This function return true if a sequence is read in successfully.
    bool readSequence(SeqType& seq)
    {
        seq_read = false;

        if(!isFileEnd())
        {
            // Read in 4 lines from FASTQ sequence file.
            if(LineReader::LinesType lines = readLines(FASTQSequence::n_fastq_sequence_lines); lines.size() == FASTQSequence::n_fastq_sequence_lines)
            {
                // Convert from string vector to string array.
                std::size_t counts = 0;
                FASTQSequenceLines seq_lines;
                for(auto& line : lines) seq_lines[counts++] = std::move(line);

                // Create a FASTQSequence-derived object from the 4 lines read.
                try
                {
                    // The temporary FASTQSequence is moved to seq.
                    seq = makeSequence(std::move(seq_lines));
                    seq_read = true;
                } catch(const std::logic_error& e)
                {
                    // Error occurs in parsing FASTQ sequence.
                    std::cerr << "Warning: " << e.what() << '!' << std::endl;
                }
            }
        }

        return seq_read;
    }

    // Read in a specified number of FASTQ sequences:
    // n_seqs: the number of FASTQ sequences to read.
    // net_count: if true, n_seqs FASTQ sequences must be retrieved successfully,
    //            otherwise, just read n_seqs FASTQ sequences with a possibility
    //            that the actual number of retrieved sequences will be less than
    //            n_seqs due to the errors in FASTQ sequences.
    //
    // Read in a specified number of FASTQ sequences in two different ways:
    // 1) Read in a specified number of FASTQ sequences, ammong which ill-
    //    formed sequences may be discarded, so that the number of sequences
    //    retrieved successfully may be less than the specified number
    //    (net_count=false).
    // 2) Read in multiple FASTQ sequences until a specified number of sequences
    //    are retrieved successfully, by discarding all ill-formed sequences.
    //    The only case where the number of the retrieved sequences is less
    //    than the specified one is that the end of FASTQ file is reached before
    //    the specified number of sequences are all retrieved (net_count=true).
    SeqTypes readSequences(std::size_t n_seqs, bool net_count=false)
    {
        SeqTypes seq_buffer;

        if(net_count)
        {
            // n_seqs is a net count of retrieved sequences.
            if(n_seqs > 0)
            {
                std::size_t counts = 0;
                SeqType seq;
                if(readSequence(seq))
                {
                    seq_buffer.push_back(std::move(seq));
                    counts++;
                }
                while(counts<n_seqs && !isFileEnd())
                {
                    // The temporary FASTQSequence is moved to seq.
                    if(readSequence(seq))
                    {
                        seq_buffer.push_back(std::move(seq));
                        counts++;
                    }
                }
            }
        }
        else
        {
            // n_seqs is a gross count of retrieved sequences.
            for(std::size_t counts = 0; counts < n_seqs; counts++)
            {
                // Read a FASTQ sequence.
                // 1) Read in a FASTQ sequence.
                //    a) If successful, add it.
                //    b) If failed, move on.
                // 2) If the file end is reached, stop reading.
                if(!isFileEnd())
                {
                    if(SeqType seq; readSequence(seq)) seq_buffer.push_back(std::move(seq));
                }
                else break;
            }
        }

        return seq_buffer;
    }
};

}

#endif /* FASTQFileReader_hpp */
