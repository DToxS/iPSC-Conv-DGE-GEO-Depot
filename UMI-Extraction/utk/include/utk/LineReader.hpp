//
//  LineReader.hpp
//  Universal-Toolkit
//
//  Created by Yuguang Xiong on 9/7/17.
//  Copyright © 2017 Granville Xiong. All rights reserved.
//

#ifndef LineReader_hpp
#define LineReader_hpp

#include <string>
#include <vector>
#include <fstream>

namespace utk
{

/// \brief Cross-platform line reader for text file
/// This class reads in a line from a text file and removes all line delimiter
/// characters:
///     Unix: \n
///     Windows: \r\n
///     Classic Macintosh: \r
class LineReader : public std::ifstream
{
public:

    using LinesType = std::vector<std::string>;

private:

    /// Name of input file
    std::string file_name;

    /// Type of line delimiter of file.
    std::string line_delim_type;

    /// Line delimiter of file.
    std::ios::char_type line_delim {'\0'};

    /// Character before the line delimiter
    std::ios::char_type pre_delim {'\0'};

    /// Flag for reaching the end of file.
    bool file_end {false};

    /// Flag for failed reading operation.
    bool read_failed {false};

private:

    /// Check if an input file is opened.
    /// If not, throw an exception.
    void checkFileOpen();

    /// Reset I/O flags to initial state.
    void resetIOFlags()
    {
        file_end = false;
        read_failed = false;
    }

    /// Clear all data member about file contents.
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

    LineReader();

    LineReader(const std::string& file_name_arg, const std::string& line_delim_type_arg);

    /// Forbid copy construction behavior.
    LineReader(const LineReader& file) = delete;

    /// Allow move construction behavior.
    LineReader(LineReader&& file);

    virtual ~LineReader() noexcept;

    /// Forbid copy assignment behavior.
    LineReader& operator=(const LineReader& file) = delete;

    /// Allow copy assignment behavior.
    LineReader& operator=(LineReader&& file);

    /// Open file and initialize parameters.
    void open(const std::string& file_name_arg, const std::string& line_delim_type_arg);

    /// Close file.
    void close();

    const std::string& getFileName() const
    {
        return file_name;
    }

    /// Check if the end of file is reached
    bool isFileEnd() const
    {
        return file_end;
    }

    /// Check if the end of file is reached
    bool isReadFailed() const
    {
        return read_failed;
    }

    /// User-level function for resetting low-level output stream to initial state.
    void resetStream()
    {
        // Reset read flags of LineReader to initial state.
        resetIOFlags();
        // Clear all error state flags of input stream.
        clear();
        // Rewind the read pointer of input stream to the beginning.
        // Note: clear() must be called before seekg(), otherwise seekg cannot
        // move the read pointer to the beginning of the input file.
        seekg(0);
    }

    /// Read a text line and remove line delimiters.
    bool readLine(std::string& line);

    /// Read multiple text lines
    LinesType readLines(std::size_t n_lines=0);
};

}

#endif /* LineReader_hpp */
