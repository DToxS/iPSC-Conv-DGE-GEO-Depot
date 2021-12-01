//
//  FASTQSequenceGroups.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 8/10/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef FASTQSequenceGroups_hpp
#define FASTQSequenceGroups_hpp

#include <map>
#include <vector>
#include <string>
#include <utility>

namespace hts
{

/// \brief FASTQ sequence groups
/// This class represents multiple groups of FASTQ sequences with a unique group
/// ID for each group by using std::map.
///
/// Template Arguments:
///
/// \tparam  SeqType   A FASTQSequence-derived class with a member function getGroupId defined.
///
/// std::map Arguments:
///
/// First: the group ID shared by all SeqType sequences for each group.
/// Second: the container of all SeqType sequences for each group.
template<typename SeqType>
class FASTQSequenceGroups : public std::map<std::string, std::vector<SeqType>>
{
private:

    using MapType = std::map<std::string, std::vector<SeqType>>;

public:

    using GroupIdType = typename MapType::key_type;
    using SequencesType = typename MapType::mapped_type;

public:

    FASTQSequenceGroups() = default;

    /// \brief Add a SeqType sequence into sequence groups.
    /// Add a SeqType sequence into sequence groups and use group ID as the key.
    /// \param  seq  A SeqType sequence.
    void addSequence(const SeqType& seq)
    {
        this->operator[](seq.getGroupId()).push_back(seq);
    }

    /// \brief Add a SeqType sequence into sequence groups.
    /// Add a SeqType sequence into sequence groups and use group ID as the key.
    /// \param  seq  A SeqType sequence.
    void addSequence(SeqType&& seq)
    {
        this->operator[](seq.getGroupId()).push_back(std::move(seq));
    }
};

}

#endif /* FASTQSequenceGroups_hpp */
