//
//  SAM-Alignment-Counter.cpp
//  SAM-Alignment-Counter
//
//  Created by Yuguang Xiong on 5/2/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <hts/SAMFileReader.hpp>
#include <hts/SAMCompositedDGEIlluminaSTARFeatureCountsAlignmentLine.hpp>
#include <hts/SAMGeneUMIAlignmentCounter.hpp>
#include <hts/SAMAlignmentPipe.hpp>
#include <utk/LineWriter.hpp>
#include <SAMAlignmentCounterArguments.hpp>

int main(int argc, const char *argv[])
{
    int exit_code = EXIT_SUCCESS;

    try
    {
        // Define some convenient types.
        using SAMDGEAlignmentLine = hts::SAMCompositedDGEIlluminaSTARFeatureCountsAlignmentLine;
        using SAMFileReader = hts::SAMFileReader<SAMDGEAlignmentLine>;
        using SAMFileWriter = utk::LineWriter;
        using SAMGeneUMIAlignmentCounter = hts::SAMGeneUMIAlignmentCounter;

        // Retrieve input arguments from command line.
        SAMAlignmentCounterArguments args(argc, argv);
        // Check input arguments.
        args.check();

        // Don't flush output stream manually.
        bool flush_ostream = false;
        // Initialize an input SAM file reader.
        SAMFileReader sam_file_reader(args.input_sam_file_path, args.parse_header_line, args.parse_header_fields, args.parse_header_fields_attribs, args.parse_align_line, args.parse_mand_align_fields, args.parse_opt_align_fields, args.parse_opt_align_fields_attribs, args.pref_opt_fields_tags, flush_ostream, args.sam_file_line_delim_type);

        // Initialize an output SAM file.
        SAMFileWriter sam_file_writer(args.output_sam_file_path);

        // Initialize a SAM alignment counter.
        SAMGeneUMIAlignmentCounter sam_align_counter;

        // Initialize a SAM aligment pipe.
        hts::SAMAlignmentPipe<SAMFileReader, SAMFileWriter, SAMDGEAlignmentLine, SAMGeneUMIAlignmentCounter> sam_align_pipe(sam_file_reader, sam_file_writer, sam_align_counter, "uniquely aligned");

        // Start processing the input SAM file and write the output.
        sam_align_pipe.run();
    }
    catch (const std::logic_error& e)
    {
        std::cerr << "Logical error: " << e.what() << std::endl;
        exit_code = EXIT_FAILURE;
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << "Runtime error: " << e.what() << std::endl;
        exit_code = EXIT_FAILURE;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Other error: " << e.what() << std::endl;
        exit_code = EXIT_FAILURE;
    }
    
    return exit_code;
}
