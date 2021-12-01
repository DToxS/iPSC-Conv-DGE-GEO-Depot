//
//  IlluminaFASTQSequence.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 5/4/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef IlluminaFASTQSequence_hpp
#define IlluminaFASTQSequence_hpp

#include <string>
#include <vector>
#include "FASTQSequence.hpp"

namespace hts
{

class IlluminaFASTQSequence : public FASTQSequence
{
public:

    // Sequence Identifier line.
    static constexpr const std::size_t n_seq_id_part_1_parts {7}, n_seq_id_part_2_parts {4};
    static constexpr const char *space_sep {" +"}, colon_sep {':'};

protected:

    // Sequence Identifier info.
    std::string seq_id_part_1, seq_id_part_2;
    std::string instrument_id;
    std::size_t run_number {0};
    std::string flowcell_id;
    std::size_t lane_number {0};
    std::size_t tile_number {0};
    std::size_t x_pos {0};
    std::size_t y_pos {0};
    std::size_t read_number {0};
    char is_filtered {'N'};
    std::size_t control_number {0};
    std::string index_sequence;

    // Level-1 check of Sequence Identifier: parts.
    bool parse_seq_id_level_1 {false};
    // Level-2 check of Sequence Identifier: fields.
    bool parse_seq_id_level_2 {false};

protected:

    // Set the group ID of FASTQ sequence using sequence information.
    virtual void setGroupId() override
    {
        group_id = index_sequence;
    }

    // Adjust level-1 and level-2 seq_id parse options according to the
    // priority: parse_seq > parse_seq_id_level_1 > parse_seq_id_level_2.
    void adjustParseOption()
    {
        if(parse_seq)
        {
            // If level-1 parsing of IlluminaFASTQSequence is not performed,
            // do not perform level-2 parsing either.
            if(!parse_seq_id_level_1) parse_seq_id_level_2 = false;
        }
        else
        {
            // If FASTQSequence is not parsed, do not parse IlluminaFASTQSequence.
            parse_seq_id_level_1 = false;
            parse_seq_id_level_2 = false;
        }
    }

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
    void reset();

public:

    /// Types of four compositing lines of an Illumina FASTQ sequence.
    enum LineType { SequenceIdentifier, Sequence, QualityIdentifier, Quality };

    IlluminaFASTQSequence();

    // parse_seq is defaulted to true for the parsing of IlluminaFASTQSequence.
    IlluminaFASTQSequence(const FASTQSequenceLines& lines, bool parse_seq=true, bool parse_seq_id_level_1=false, bool parse_seq_id_level_2=false, bool flush_ostream=false);

    // parse_seq is defaulted to true for the parsing of IlluminaFASTQSequence.
    IlluminaFASTQSequence(FASTQSequenceLines&& lines, bool parse_seq=true, bool parse_seq_id_level_1=false, bool parse_seq_id_level_2=false, bool flush_ostream=false);

    // parse_seq is defaulted to true for the parsing of IlluminaFASTQSequence.
    IlluminaFASTQSequence(const std::string& line1, const std::string& line2, const std::string& line3, const std::string& line4, bool parse_seq=true, bool parse_seq_id_level_1=false, bool parse_seq_id_level_2=false, bool flush_ostream=false);

    // parse_seq is defaulted to true for the parsing of IlluminaFASTQSequence.
    IlluminaFASTQSequence(std::string&& line1, std::string&& line2, std::string&& line3, std::string&& line4, bool parse_seq=true, bool parse_seq_id_level_1=false, bool parse_seq_id_level_2=false, bool flush_ostream=false);

    IlluminaFASTQSequence(const FASTQSequence& seq, bool parse_seq_id_level_1=false, bool parse_seq_id_level_2=false);

    IlluminaFASTQSequence(FASTQSequence&& seq, bool parse_seq_id_level_1=false, bool parse_seq_id_level_2=false);

    IlluminaFASTQSequence(const IlluminaFASTQSequence& seq);

    IlluminaFASTQSequence(IlluminaFASTQSequence&& seq);

    virtual ~IlluminaFASTQSequence() noexcept;

    IlluminaFASTQSequence& operator=(const IlluminaFASTQSequence& seq);

    IlluminaFASTQSequence& operator=(IlluminaFASTQSequence&& seq);

    const std::string& getSequenceIdentifierPart1() const
    {
        return seq_id_part_1;
    }

    std::string& getSequenceIdentifierPart1()
    {
        return seq_id_part_1;
    }

    const std::string& getSequenceIdentifierPart2() const
    {
        return seq_id_part_2;
    }

    std::string& getSequenceIdentifierPart2()
    {
        return seq_id_part_2;
    }

    const std::string& getInstrumentId() const
    {
        return instrument_id;
    }

    std::string& getInstrumentId()
    {
        return instrument_id;
    }

    std::size_t getRunNumber() const
    {
        return run_number;
    }

    const std::string& getFlowcellId() const
    {
        return flowcell_id;
    }

    std::string& getFlowcellId()
    {
        return flowcell_id;
    }

    std::size_t getLaneNumber() const
    {
        return lane_number;
    }

    std::size_t getTileNumber() const
    {
        return tile_number;
    }

    std::size_t getXPos() const
    {
        return x_pos;
    }

    std::size_t getYPos() const
    {
        return y_pos;
    }

    std::size_t getReadNumber() const
    {
        return read_number;
    }

    char getIsFiltered() const
    {
        return is_filtered;
    }

    std::size_t getControlNumber() const
    {
        return control_number;
    }

    const std::string& getIndexSequence() const
    {
        return index_sequence;
    }

    std::string& getIndexSequence()
    {
        return index_sequence;
    }

    bool getParseSequenceIdentifierLevel1() const
    {
        return parse_seq_id_level_1;
    }

    bool getParseSequenceIdentifierLevel2() const
    {
        return parse_seq_id_level_2;
    }

    void setParseSequenceIdentifierLevel1(bool parse_seq_id_level_1_val)
    {
        parse_seq_id_level_1 = parse_seq_id_level_1_val;
        // Adjust IlluminaFASTQSequence parsing options.
        adjustParseOption();
    }

    void setParseSequenceIdentifierLevel2(bool parse_seq_id_level_2_val)
    {
        parse_seq_id_level_2 = parse_seq_id_level_2_val;
        // Adjust IlluminaFASTQSequence parsing options.
        adjustParseOption();
    }

    void setParseOption(bool parse_seq_id_level_1_val, bool parse_seq_id_level_2_val)
    {
        parse_seq_id_level_1 = parse_seq_id_level_1_val;
        parse_seq_id_level_2 = parse_seq_id_level_2_val;
        // Adjust IlluminaFASTQSequence parsing options.
        adjustParseOption();
    }

    // Parse the information specific to Illumina FASTQ sequence.
    void parse();
};

using IlluminaFASTQSequences = std::vector<IlluminaFASTQSequence>;

}

#endif /* IlluminaFASTQSequence_hpp */
