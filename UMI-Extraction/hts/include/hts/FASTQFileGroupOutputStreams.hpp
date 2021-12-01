//
//  FASTQFileGroupOutputStreams.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 8/14/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef FASTQFileGroupOutputStreams_hpp
#define FASTQFileGroupOutputStreams_hpp

#include <map>
#include <string>
#include <fstream>
#include <stdexcept>
#include <type_traits>
#include "FASTQSequence.hpp"
#include "WellBarcodeTable.hpp"

namespace hts
{

/// \brief Output file streams for single-end demultiplexed FASTQ sequences
///
/// std::map Arguments:
///
/// First: the group ID of FASTQ file stream.
/// Second: the single-end FASTQ file stream.
class FASTQFileGroupOutputStreams : public std::map<std::string, std::ofstream>
{
public:

    using GroupIdType = key_type;
    using FileStreamType = mapped_type;

public:

    FASTQFileGroupOutputStreams() = default;

    FASTQFileGroupOutputStreams(const std::string& main_file_name, const std::string& file_dir, const WellBarcodeTable& well_barcode_table);

    template <typename SeqType>
    void writeSequence(const SeqType& seq, const GroupIdType& group_id)
    {
        if constexpr (std::is_base_of_v<FASTQSequence, SeqType>) operator[](group_id) << seq << '\n';
        else throw std::logic_error("Cannot write non-FASTQSequence object");
    }

    void flush(const GroupIdType& group_id)
    {
        operator[](group_id).flush();
    }
};

}

#endif /* FASTQFileGroupOutputStreams_hpp */
