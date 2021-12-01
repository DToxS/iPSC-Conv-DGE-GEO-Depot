//
//  PairedFASTQFileGroupOutputStreams.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 8/14/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef PairedFASTQFileGroupOutputStreams_hpp
#define PairedFASTQFileGroupOutputStreams_hpp

#include <map>
#include <string>
#include <fstream>
#include <stdexcept>
#include <type_traits>
#include "IlluminaFASTQSequence.hpp"
#include "PairedFASTQSequenceCreator.hpp"
#include "WellBarcodeTable.hpp"

namespace hts
{

/// \brief Output file streams for paired-end demultiplexed FASTQ sequences
///
/// std::map Arguments:
///
/// First: the group ID of FASTQ file stream.
/// Second: the paired-end FASTQ file stream.
class PairedFASTQFileGroupOutputStreams : public std::map<std::string, std::pair<std::ofstream, std::ofstream>>
{
public:

    using GroupIdType = key_type;
    using PairedFileStreamType = mapped_type;
    using FileStreamType = mapped_type::first_type;

public:

    PairedFASTQFileGroupOutputStreams() = default;

    PairedFASTQFileGroupOutputStreams(const std::string& main_file_name, const std::string& file_dir, const WellBarcodeTable& well_barcode_table);

    template <typename SeqType>
    void writeSequence(const PairedFASTQSequenceCreator<SeqType>& seq, const GroupIdType& group_id)
    {
        if constexpr (std::is_base_of_v<IlluminaFASTQSequence, SeqType>)
        {
            operator[](group_id).first << seq.getSequence1();
            operator[](group_id).second << seq.getSequence2();
        }
        else throw std::logic_error("Cannot write non-IlluminaFASTQSequence paired objects");
    }

    void flush(const GroupIdType& group_id)
    {
        operator[](group_id).first.flush();
        operator[](group_id).second.flush();
    }
};

}

#endif /* PairedFASTQFileGroupOutputStreams_hpp */
