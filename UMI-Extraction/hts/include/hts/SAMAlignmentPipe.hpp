//
//  SAMAlignmentPipe.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 3/30/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#ifndef SAMAlignmentPipe_hpp
#define SAMAlignmentPipe_hpp

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "SAMHeaderDataLine.hpp"
#include "SAMHeaderCommentLine.hpp"
#include "SAMAlignmentLine.hpp"

namespace hts
{

/// \brief Pipe all alignments from SAM file to alignment counter
/// This class reads in all lines from a SAM alignment file and parses them
/// with the functions corresponding to header data line, header comment line,
/// and alignment line before writing to a parsed SAM file.
///
/// Template Arguments:
///
/// \tparam  SAMFileReaderType         A SAMFileReader-based SAM file reader.
/// \tparam  SAMFileWriterType         A LineWriter-based SAM file writer.
/// \tparam  SAMAlignmentLineType      A SAMAlignmentLine-based SAM alignment line.
/// \tparam  SAMAlignmentCounterType   A SAMAlignmentCounter-based counter class for SAM alignment lines.
///
/// \note    SAMAlignmentLineType and SAMAlignmentCounterType must be compatible
///          with each, because SAMAlignmentCounterType needs to call functions
///          specific to SAMAlignmentLineType.
template<typename SAMFileReaderType, typename SAMFileWriterType, typename SAMAlignmentLineType, typename SAMAlignmentCounterType>
class SAMAlignmentPipe
{
private:

    /// A holder of SAM file reader.
    SAMFileReaderType& file_reader;

    /// A holder of SAM file writer.
    SAMFileWriterType& file_writer;

    /// A holder of SAM alignment counter.
    SAMAlignmentCounterType& align_counter;

    /// Keyword for auxiliary alignment line to read.
    std::string read_aux_align_line_name;

    /// Keyword for auxiliary header data line to read.
    std::string read_aux_header_data_line_name;

    /// Keyword for auxiliary header comment line to read.
    std::string read_aux_header_comment_line_name;

public:

    SAMAlignmentPipe(SAMFileReaderType& reader, SAMFileWriterType& writer, SAMAlignmentCounterType& counter, const std::string& read_aux_align_line_name = "auxiliary", const std::string& read_aux_header_data_line_name = "auxiliary", const std::string& read_aux_header_comment_line_name = "auxiliary") : file_reader{reader}, file_writer{writer}, align_counter{counter}, read_aux_align_line_name{read_aux_align_line_name}, read_aux_header_data_line_name{read_aux_header_data_line_name}, read_aux_header_comment_line_name{read_aux_header_comment_line_name} {}

    /// \brief Parse all lines of a SAM file to remove duplicate alignments.
    ///
    /// Parse all lines of a SAM file to filter out non-uniquely aligned and
    /// non-uniquely tagged sequence alignments. The output SAM file contains:
    ///
    /// 1) All header data lines.
    /// 2) All header comment lines.
    /// 3) Alignments lines for all uniquely aligned sequences tagged with
    ///    distinct UMI barcodes for each gene.
    std::size_t run()
    {
        // The number of input alignment lines.
        std::size_t n_read_align_lines {0};

        // The number of auxiliary input alignment lines.
        std::size_t n_read_aux_align_lines {0};

        // The number of output alignment lines.
        std::size_t n_write_align_lines {0};

        // The number of input header data lines.
        std::size_t n_read_header_data_lines {0};

        // The number of auxiliary input header data lines.
        std::size_t n_read_aux_header_data_lines {0};

       // The number of output header data lines.
        std::size_t n_write_header_data_lines {0};

        // The number of input header comment lines.
        std::size_t n_read_header_comment_lines {0};

        // The number of auxiliary input header comment lines.
        std::size_t n_read_aux_header_comment_lines {0};

        // The number of output header comment lines.
        std::size_t n_write_header_comment_lines {0};

        // Read each line from the input SAM file and use SAMAlignmentCounterType
        // to decide whether to write this line to the output SAM file.
        for(std::string line; file_reader.readLine(line);)
        {
            // Process alignment line.
            if(line.front() != SAMHeaderLine::getBeginChar())
            {
                // Create a SAMAlignmentLineType object.
                if(SAMAlignmentLineType alignment_line; file_reader.template readAlignmentLine<false>(line, alignment_line))
                {
                    bool aux_count = false;
                    if(align_counter.countAlignmentLine(alignment_line, aux_count))
                    {
                        file_writer.writeLine(alignment_line);
                        n_write_align_lines++;
                    }
                    if(aux_count) n_read_aux_align_lines++;
                }
                n_read_align_lines++;
            }
            else
            {
                // Process header data line.
                if(const std::string& comment_record_type = SAMHeaderCommentLine::getStdSAMCommentHeaderRecordType(); line.substr(0,comment_record_type.length()) != comment_record_type)
                {
                    if(SAMHeaderDataLine data_line; file_reader.template readHeaderDataLine<false>(line, data_line))
                    {
                        bool aux_count = false;
                        if(align_counter.countHeaderDataLine(data_line, aux_count))
                        {
                            file_writer.writeLine(data_line);
                            n_write_header_data_lines++;
                        }
                        if(aux_count) n_read_aux_header_data_lines++;
                    }
                    n_read_header_data_lines++;
                }
                // Process header comment line.
                else
                {
                    if(SAMHeaderCommentLine comment_line; file_reader.template readHeaderCommentLine<false>(line, comment_line))
                    {
                        bool aux_count = false;
                        if(align_counter.countHeaderCommentLine(comment_line, aux_count))
                        {
                            file_writer.writeLine(comment_line);
                            n_write_header_comment_lines++;
                        }
                        if(aux_count) n_read_aux_header_comment_lines++;
                    }
                    n_read_header_comment_lines++;
                }
            }
        }

        // Calculate the statistics of input and output lines.
        std::size_t n_read_lines = n_read_header_data_lines + n_read_header_comment_lines + n_read_align_lines;
        std::size_t n_write_lines = n_write_header_data_lines + n_write_header_comment_lines + n_write_align_lines;
        std::cout << "Read " << n_read_header_data_lines << " header data lines" << '\n';
        std::cout << "Read " << n_read_aux_header_data_lines << ' ' << read_aux_header_data_line_name << " header data lines" << '\n';
        std::cout << "Write " << n_write_header_data_lines << " selected header data lines" << '\n';
        std::cout << "Read " << n_read_header_comment_lines << " header comment lines" << '\n';
        std::cout << "Read " << n_read_aux_header_comment_lines << ' ' << read_aux_header_comment_line_name << " header comment lines" << '\n';
        std::cout << "Write " << n_write_header_comment_lines << " selected header comment lines" << '\n';
        std::cout << "Read " << n_read_align_lines << " sequence alignment lines" << '\n';

        std::cout << "Read " << n_read_aux_align_lines << ' ' << read_aux_align_line_name << " sequence alignment lines" << '\n';
        std::cout << "Write " << n_write_align_lines << " selected sequence alignment lines" << '\n';
        std::cout << "Read " << n_read_lines << " lines in total" << '\n';
        std::cout << "Write " << n_write_lines << " selected lines in total" << '\n';

        // Return the number of uniquely aligned and agged sequence alignments.
        return n_read_lines;
    }
};

}

#endif /* SAMAlignmentPipe_hpp */
