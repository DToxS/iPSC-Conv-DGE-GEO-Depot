//
//  FASTQSequence.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 5/2/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef FASTQSequence_hpp
#define FASTQSequence_hpp

#include <string>
#include <array>
#include <vector>
#include <utility>
#include <ostream>
#include <cstddef>

namespace hts
{

class FASTQSequence
{
public:

    /// Types of four compositing lines of a FASTQ sequence.
    enum LineType { Identifier, Sequence, Option, Quality };

    /// The beginning character of Identifier line.
    static constexpr char id_line_beg_char {'@'}, opt_line_beg_char {'+'};

    // Number of FASTQ sequence lines.
    static constexpr const std::size_t n_fastq_sequence_lines {4};

    // Data type of FASTQ sequence.
    using FASTQSequenceLines = std::array<std::string, n_fastq_sequence_lines>;

protected:

    // A four-line FASTQ sequence.
    FASTQSequenceLines lines;

    // Read length of FASTQ sequence.
    std::size_t read_length {0};

    // Group ID of FASTQ sequence.
    std::string group_id;

    // Validate FASTQ sequence format.
    bool parse_seq {false};

    /// \brief Flush each written sequence line from output stream to disk.
    /// Note: this option needs to be switched on in a multi-threading envronment.
    bool flush_ostream {false};

protected:

    // Set the group ID of FASTQ sequence using sequence information.
    // Note: this function needs to be overridden in derived classes.
    virtual void setGroupId() {}

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

    FASTQSequence();

    FASTQSequence(const FASTQSequenceLines& lines, bool parse_seq=false, bool flush_ostream=false);

    FASTQSequence(FASTQSequenceLines&& lines, bool parse_seq=false, bool flush_ostream=false);

    FASTQSequence(const std::string& line1, const std::string& line2, const std::string& line3, const std::string& line4, bool parse_seq=true, bool flush_ostream=false);

    FASTQSequence(std::string&& line1, std::string&& line2, std::string&& line3, std::string&& line4, bool parse_seq=true, bool flush_ostream=false);

    FASTQSequence(const FASTQSequence& seq);

    FASTQSequence(FASTQSequence&& seq);

    virtual ~FASTQSequence() noexcept;

    FASTQSequence& operator=(const FASTQSequence& seq);

    FASTQSequence& operator=(FASTQSequence&& seq);

    std::string& operator[](std::size_t idx)
    {
        return lines[idx];
    }

    const std::string& operator[](std::size_t idx) const
    {
        return lines[idx];
    }

    std::size_t getNLines() const
    {
        return n_fastq_sequence_lines;
    }

    const std::string& getGroupId() const
    {
        return group_id;
    }

    std::string& getGroupId()
    {
        return group_id;
    }

    std::size_t getReadLength() const
    {
        return read_length;
    }

    bool getParseSequence() const
    {
        return parse_seq;
    }

    void setParseSequence(bool parse_seq_val)
    {
        parse_seq = parse_seq_val;
    }

    bool getFlushOstream() const
    {
        return flush_ostream;
    }

    void setFlushOstream(bool flush_ostream_val)
    {
        flush_ostream = flush_ostream_val;
    }

    const std::string& getIdentifierLine() const
    {
        return lines[Identifier];
    }

    std::string& getIdentifierLine()
    {
        return lines[Identifier];
    }

    const std::string& getSequenceLine() const
    {
        return lines[Sequence];
    }

    std::string& getSequenceLine()
    {
        return lines[Sequence];
    }

    const std::string& getOptionLine() const
    {
        return lines[Option];
    }

    std::string& getOptionLine()
    {
        return lines[Option];
    }

    const std::string& getQualityLine() const
    {
        return lines[Quality];
    }

    std::string& getQualityLine()
    {
        return lines[Quality];
    }

    const FASTQSequenceLines& getLines() const
    {
        return lines;
    }

    FASTQSequenceLines& getLines()
    {
        return lines;
    }

    bool empty() const;

    // Parse the information specific to Illumina FASTQ sequence.
    void parse();
};

using FASTQSequences = std::vector<FASTQSequence>;
using FASTQSequenceLines = FASTQSequence::FASTQSequenceLines;

// Output of FASTQ sequence.
std::ostream& operator<<(std::ostream& os, const FASTQSequence& seq);

}

#endif /* FASTQSequence_hpp */
