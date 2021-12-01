//
//  FASTQSequenceDemuxer.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 8/10/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef FASTQSequenceDemuxer_hpp
#define FASTQSequenceDemuxer_hpp

#include <iostream>
#include <utility>
#include "FASTQSequenceGroups.hpp"
#include "WellBarcodeReader.hpp"

namespace hts
{

/// \brief Demultiplexer of FASTQ sequence
/// This class represents a demultiplexer of FASTQ sequences that assigns a given
/// FASTQ sequence to a corresponding group specified by its group ID if the ID
/// matches a well barcode in a table of well barcode (key) and well number (value).
/// Each group of grouped FASTQ sequences is then denoted by the well number
/// of the corresponding well barcode (that is the group ID).
///
/// Template Arguments:
///
/// \tparam  OutputStreamsType  A group of output streams for corresponding FASTQ sequence.
/// \tparam  SeqType            A FASTQSequence-based class with getGroupId function defined.
template<typename OutputStreamsType, typename SeqType>
class FASTQSequenceDemuxer
{
public:

    using FASTQSequenceGroupsType = FASTQSequenceGroups<SeqType>;

protected:
    
    using SequencesType = typename FASTQSequenceGroupsType::SequencesType;
    using GroupIdType = typename FASTQSequenceGroupsType::GroupIdType;

protected:

    /// \brief Output streams for demultiplexed grouped sequences
    /// Each demultiplexed sequence is written to an output stream with matched
    /// group ID.
    OutputStreamsType output_streams;

    /// \brief SeqType sequence groups
    /// Each group include a group ID and multiple sequences with that ID.
    /// Note: for the sake of convenience, the group ID of a sequence is mapped
    /// to cooresponding well number according to well barcode table and then
    /// stored into the sequence groups.
    FASTQSequenceGroupsType seq_groups;

    /// \brief Well barcode table
    /// An association between well barcode (key) and corresponding well number
    /// (value), where the well barcode will be used to match the group ID of
    /// SeqType sequence.
    WellBarcodeTable well_barcode_table;

    /// \brief Capacity of sequence groups
    /// A uniform capacity of all sequence groups in terms of the maximum number
    /// of SeqType sequences that any group can hold.
    std::size_t n_max_seqs {0};

    /// \brief Flush written sequences from output stream to disk.
    /// A flag of controlling wheather or not flush written sequences contained
    /// in output stream to disk when the capacity of sequence groups is reached.
    bool flush_ostream {true};

    /// \brief Switch for printing verbose information
    bool verbose {false};

    /// \brief Number of grouped and un-grouped sequences
    /// The number of FASTQ sequences that can or cannot be grouped according to
    /// well barcode table.
    std::size_t n_grouped_seqs {0}, n_ungrouped_seqs {0};

protected:
    
    /// \brief Initialize sequence groups
    /// Initialize the groups of SeqType sequences by well numbers.
    void initSequenceGroups()
    {
        for(const auto& well_barcode : well_barcode_table)
        {
            seq_groups[well_barcode.second] = SequencesType();
        }
    }

public:

    FASTQSequenceDemuxer(const std::string& table_file_path, const std::string& main_file_name, const std::string& file_dir, std::size_t max_seqs=0, bool flush=true, const std::string& line_delim_type="unix", bool verb=false) : n_max_seqs{max_seqs}, flush_ostream{flush}, verbose{verb}
    {
        // Initialize well barcode table.
        WellBarcodeReader well_barcode_reader(table_file_path, line_delim_type);
        well_barcode_table = well_barcode_reader.read();
        // Initialize output streams for demultiplexed grouped sequences.
        output_streams = OutputStreamsType(main_file_name, file_dir, well_barcode_table);
        // Initialize the groups of SeqType sequences by well numbers.
        initSequenceGroups();
    }

    FASTQSequenceDemuxer(const WellBarcodeTable& table, OutputStreamsType&& ostreams, std::size_t max_seqs=0, bool flush=true, bool verb=false) : output_streams{std::move(ostreams)}, well_barcode_table{table}, n_max_seqs{max_seqs}, flush_ostream{flush}, verbose{verb}
    {
        // Initialize the groups of SeqType sequences by well numbers.
        initSequenceGroups();
    }

    FASTQSequenceDemuxer(const FASTQSequenceDemuxer&) = delete;

    FASTQSequenceDemuxer& operator=(const FASTQSequenceDemuxer&) = delete;

    /// \brief Add a SeqType sequence to sequence groups
    /// Add a SeqType sequence into the sequence groups.
    /// \param  seq  A SeqType sequence.
    template<typename T>
    void addSequence(T&& seq)
    {
        // Determine if the group ID of the sequence can be found in the well
        // barcode table. If found, add this sequence into the sequence groups.
        if(auto search = well_barcode_table.find(seq.getGroupId()); search != well_barcode_table.end())
        {
            // Get actual group ID used in the sequence gorups.
            const auto& group_id = search->second;
            // Get the sequence container of the maxout group.
            auto& group_seqs = seq_groups[group_id];
            // Write the sequence group if it's maxout.
            if(group_seqs.size() >= n_max_seqs) writeSequences(group_id, group_seqs, flush_ostream);
            // Add a SeqType sequence to the sequence group.
            if(n_max_seqs==0 || (n_max_seqs>0 && group_seqs.size()<n_max_seqs))
            {
                // The sequence is added into a corresponding group.
                group_seqs.push_back(std::forward<T>(seq));
                ++n_grouped_seqs;
            }
        }
        else
        {
            // The sequence cannot be added into any group because no corresponding
            // group is found.
            ++n_ungrouped_seqs;
        }
    }

    /// \brief Create and add a SeqType sequence
    /// Use template arguments to create a SeqType sequence and add it into
    /// the sequence groups.
    /// \param  args  arbitrary arguments needed by sequence creation.
    template<typename... Args>
    void addSequence(Args&&... args)
    {
        // Create a SeqType sequence using template arguments and add it into
        // the sequence groups.
        addSequence(SeqType(std::forward<Args>(args)...));
    }

    /// \brief Write all sequence groups
    /// Output all the sequences contained in all groups and then clear all
    /// groups.
    void writeSequences(bool flush=false)
    {
        for(auto& seq_group : seq_groups)
        {
            // Write all the sequences in the maxout group to output file.
            if(auto& group_seqs = seq_group.second; group_seqs.size() > 0) writeSequences(seq_group.first, group_seqs, flush);
        }
    }

    /// \brief Write specified sequence group
    /// Output all the sequences contained in a maxout group and then clear the
    /// group.
    /// \param  group_id  A group ID.
    /// \param  group_seqs  A container of sequences with the same group ID.
    /// \param  flush  Whether to flush written sequences in output streams.
    void writeSequences(const GroupIdType& group_id, SequencesType& group_seqs, bool flush=false)
    {
        // Output all the sequences in the maxout group to standard output.
        for(const auto& group_seq : group_seqs)
        {
            // Write all the sequences in the maxout group to output file.
            // Note:
            // 1) No need to flush output stream every time after writing a sequence in a
            // signle threading environment.
            // 2) In a multithreading environment where multiple threads write sequences
            // to the same output stream, then the stream must be flushed after each output
            // operation, aka. flush after writing each single line of a single sequence.
            output_streams.writeSequence(group_seq, group_id);
            // Print all the sequences in the maxout group to standard output.
            if(verbose) std::cout << group_seq;
        }
        // Flush output file.
        if(flush)
        {
            output_streams.flush(group_id);
            if(verbose) std::cout.flush();
        }

        // Clear maxout sequence group.
        group_seqs.clear();
    }

    const FASTQSequenceGroupsType& getSequenceGroups() const
    {
        return seq_groups;
    }

    FASTQSequenceGroupsType& getSequenceGroups()
    {
        return seq_groups;
    }

    std::size_t getNumberOfSequenceGroups() const
    {
        return seq_groups.size();
    }

    std::size_t getNumberOfGroupedSequences() const
    {
        return n_grouped_seqs;
    }

    std::size_t getNumberOfUngroupedSequences() const
    {
        return n_ungrouped_seqs;
    }
};

}

#endif /* FASTQSequenceDemuxer_hpp */
