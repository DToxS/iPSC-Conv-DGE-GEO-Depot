//
//  LineWriter.hpp
//  Universal-Toolkit
//
//  Created by Yuguang Xiong on 3/30/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#ifndef LineWriter_hpp
#define LineWriter_hpp

#include <string>
#include <fstream>

namespace utk
{

class LineWriter : public std::ofstream
{
private:

    /// Name of output file.
    std::string file_name;

    /// Line delimiter.
    std::ios::char_type line_delim {'\0'};

    /// Flag for failed writing operation.
    bool write_failed {false};

private:

    /// Check if an output file is opened.
    /// If not, throw an exception.
    void checkFileOpen();

    /// Reset I/O flags to initial state.
    void resetIOFlags()
    {
        write_failed = false;
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

    LineWriter();

    LineWriter(const std::string& file_name_arg, std::ios::char_type line_delim_arg='\n');

    /// Forbid copy construction behavior.
    LineWriter(const LineWriter& file) = delete;

    /// Allow move construction behavior.
    LineWriter(LineWriter&& file);

    virtual ~LineWriter() noexcept;

    /// Forbid copy assignment behavior.
    LineWriter& operator=(const LineWriter& file) = delete;

    /// Allow copy assignment behavior.
    LineWriter& operator=(LineWriter&& file);

    /// Open file and initialize parameters.
    void open(const std::string& file_name_arg);

    /// Close file.
    void close();

    const std::string& getFileName() const
    {
        return file_name;
    }

    /// Check if the end of file is reached
    bool isWriteFailed() const
    {
        return write_failed;
    }

    /// User-level function for resetting low-level output stream to initial state.
    void resetStream()
    {
        // Reset write flags of LineWriter to initial state.
        resetIOFlags();
        // Clear all error state flags of output stream.
        clear();
        // Rewind the write pointer of output stream to the beginning.
        // Note: clear() must be called before seekg(), otherwise seekg cannot
        // move the read pointer to the beginning of the output file.
        seekp(0);
    }

    /// Write a text line.
    /// Note: the line type T must have overloaded the operator function
    /// std::ostream& operator<<(std::ostream& os, const T& line)
    template<typename T>
    bool writeLine(const T& line)
    {
        // Write a line followed by a line delim.
        *this << line;

        // Write line delimiter as needed.
        if(line_delim != '\0') *this << widen(line_delim);

        // Set the flag if write operation fails.
        if(bad()) write_failed = true;

        return write_failed;
    }
};

}

#endif /* LineWriter_hpp */
