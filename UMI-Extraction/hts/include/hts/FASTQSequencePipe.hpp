//
//  FASTQSequencePipe.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 8/25/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef FASTQSequencePipe_hpp
#define FASTQSequencePipe_hpp

#include <iostream>
#include <stdexcept>

namespace hts
{

/// \brief Pipe all sequences from FASTQ file to sequence demultiplexer
/// This class reads all sequence fragments from a multiplexed FASTQ file and
/// send them to a sequence demultiplexer that demultiplex sequences fragments
/// into corresponding FASTQ files.
///
/// Template Arguments:
///
/// \tparam  FileReader   A FASTQFileReader-derived class with readSequences and
///                       isFileEnd member functions defined.
/// \tparam  SeqDemuxer   A FASTQ sequence demultiplexer with addSequence() member
///                       function defined.
template<typename FileReader, typename SeqDemuxer>
class FASTQSequencePipe
{
private:

    /// \brief FASTQ file reader
    /// A holder of FASTQ file reader.
    FileReader& file_reader;

    /// \brief FASTQ sequence demultiplexer
    /// A holder of FASTQ sequence demultiplexer.
    SeqDemuxer& seq_demuxer;

public:

    FASTQSequencePipe(FileReader& reader, SeqDemuxer& demuxer) : file_reader{reader}, seq_demuxer{demuxer} {}

    /// \brief Feed all FASTQ sequences from file to demultiplexer
    /// This function reads all FASTQ sequence fragments from a specified FASTQ
    /// file reader and feeds each sequence to a specified sequence demultiplexer
    /// with required arguments.
    ///
    /// Template Arguments:
    ///
    /// \tparam  ArgTypes   All arguments required by demultiplexer for verifying
    ///                     and creating a FASTQ sequence correctly assembled from
    ///                     the read raw sequence.
    template<typename... ArgTypes>
    void run(std::size_t n_read_seqs, ArgTypes&&... args)
    {
        // Read multiple FASTQ sequences before reaching the end of FASTQ file.
        while(!file_reader.isFileEnd())
        {
            // If the end of FASTQ file is not reached yet, keep reading in
            // next batch of sequences.
            if(auto seqs = file_reader.readSequences(n_read_seqs); seqs.size()>0)
            {
                for(auto iter=seqs.begin(); iter!=seqs.end(); ++iter)
                {
                    try
                    {
                        // Assemble a FASTQ sequence and add it to corresponding
                        // sequence group in demultiplexer.
                        seq_demuxer.addSequence(std::move(*iter), std::forward<ArgTypes>(args)...);
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

#endif /* FASTQSequencePipe_hpp */
