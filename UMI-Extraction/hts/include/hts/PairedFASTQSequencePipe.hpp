//
//  PairedFASTQSequencePipe.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 8/25/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef PairedFASTQSequencePipe_hpp
#define PairedFASTQSequencePipe_hpp

#include <iostream>
#include <stdexcept>

namespace hts
{

/// \brief Pipe all sequences from paired FASTQ files to sequence demultiplexer
/// This class reads all sequence fragments from a pair of multiplexed FASTQ file
/// and send them to a sequence demultiplexer that demultiplex sequences fragments
/// into corresponding FASTQ files.
///
/// Template Arguments:
///
/// \tparam  FileReader   A FASTQFileReader-derived class with readSequences and
///                       isFileEnd member functions defined.
/// \tparam  SeqDemuxer   A FASTQ sequence demultiplexer with addSequence() member
///                       function defined.
template<typename FileReader, typename SeqDemuxer>
class PairedFASTQSequencePipe
{
private:

    /// \brief FASTQ file reader
    /// A holder of FASTQ file reader.
    FileReader &file_reader_1, &file_reader_2;

    /// \brief FASTQ sequence demultiplexer
    /// A holder of FASTQ sequence demultiplexer.
    SeqDemuxer& seq_demuxer;

public:

    PairedFASTQSequencePipe(FileReader& reader_1, FileReader& reader_2, SeqDemuxer& demuxer) : file_reader_1{reader_1}, file_reader_2{reader_2}, seq_demuxer{demuxer} {}

    /// \brief Feed all FASTQ sequences from file to demultiplexer
    /// This function reads all FASTQ sequence fragments from a pair of specified
    /// FASTQ file readers and feeds each pair of sequences to a specified sequence
    /// demultiplexer with required arguments.
    ///
    /// Template Arguments:
    ///
    /// \tparam  ArgTypes   All arguments required by demultiplexer for verifying
    ///                     and creating a FASTQ sequence or a pair of sequences
    ///                     correctly assembled from the read raw sequence pair.
    template<typename... ArgTypes>
    void run(std::size_t n_read_seqs, ArgTypes&&... args)
    {
        // Read multiple FASTQ sequences before reaching the end of FASTQ file.
        while(!file_reader_1.isFileEnd())
        {
            // If the end of FASTQ file is not reached yet, keep reading in
            // next batch of sequences.
            if(auto seqs_1 = file_reader_1.readSequences(n_read_seqs), seqs_2 = file_reader_2.readSequences(n_read_seqs); seqs_1.size()>0 && seqs_2.size()>0)
            {
                for(auto iter_1=seqs_1.begin(),iter_2=seqs_2.begin(); iter_1!=seqs_1.end()&&iter_2!=seqs_2.end(); ++iter_1,++iter_2)
                {
                    try
                    {
                        // Assemble a FASTQ sequence or a pair of sequences and add it to
                        // corresponding sequence group in demultiplexer.
                        seq_demuxer.addSequence(std::move(*iter_1), std::move(*iter_2), std::forward<ArgTypes>(args)...);
                    } catch (const std::logic_error& e)
                    {
                        // Error occurs when adding a new FASTQ sequence to demultiplxer.
                        std::cerr << "Warning: " << e.what() << '!' << std::endl;
                    }
                }
            }
        }
    }
};

}

#endif /* PairedFASTQSequencePipe_hpp */
