//
//  SAMAlignmentCounterArguments.cpp
//  SAM-Alignment-Counter
//
//  Created by Granville Xiong on 4/1/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <utk/SystemProperties.hpp>
#include <utk/StringUtils.hpp>
#include <utk/FileUtils.hpp>
#include <SAMAlignmentCounterArguments.hpp>

/// Retrieve input arguments.
SAMAlignmentCounterArguments::SAMAlignmentCounterArguments(int argc, const char** argv) :
    utk::ProgramArguments(argc, argv, 3, 12),
    parse_header_line{false},
    parse_header_fields{false},
    parse_header_fields_attribs{false},
    parse_align_line{true},
    parse_mand_align_fields{false},
    parse_opt_align_fields{true},
    parse_opt_align_fields_attribs{false},
    use_pref_opt_fields{true},
    sam_file_line_delim_type{"unix"},
    preset_pref_opt_fields_tags{"XS","XN","XT"} {}

/// Assign mandatory input arguments
void SAMAlignmentCounterArguments::assignMandatoryArguments()
{
    // Assign mandatory arguments.
    input_sam_file_path = argv[1];
    output_sam_file_path = argv[2];
}

/// Assign optional input arguments
void SAMAlignmentCounterArguments::assignOptionalArguments()
{
    // Assign optional arguments.
    // 3rd argument.
    if(argc > 3) parse_header_line = utk::convert<bool>(argv[3]);
    // 4th argument.
    if(argc > 4) parse_header_fields = utk::convert<bool>(argv[4]);
    // 5th argument.
    if(argc > 5) parse_header_fields_attribs = utk::convert<bool>(argv[5]);
    // 6th argument.
    if(argc > 6) parse_align_line = utk::convert<bool>(argv[6]);
    // 7th argument.
    if(argc > 7) parse_mand_align_fields = utk::convert<bool>(argv[7]);
    // 8th argument.
    if(argc > 8) parse_opt_align_fields = utk::convert<bool>(argv[8]);
    // 9th argument.
    if(argc > 9) parse_opt_align_fields_attribs = utk::convert<bool>(argv[9]);
    // 10th argument.
    if(argc > 10) use_pref_opt_fields = utk::convert<bool>(argv[10]);
    // 11th argument.
    if(argc > 11) sam_file_line_delim_type = utk::toLowerString(argv[11]);
}

/// Check input arguments.
void SAMAlignmentCounterArguments::validateArguments()
{
    // Check the path of the input SAM file.
    utk::checkFileReadability(input_sam_file_path);

    // Check the type of line delimiter of SAM file.
    if(sam_file_line_delim_type != "windows" && sam_file_line_delim_type != "unix" && sam_file_line_delim_type != "macintosh")
    {
        throw std::logic_error("Line Delimiter Type of Input SAM File must be one of: unix, windows, or macintosh");
    }

    // Set the tags of preferred optional fields to be parsed according to
    // input argument use_pref_opt_fields.
    if(use_pref_opt_fields) pref_opt_fields_tags = preset_pref_opt_fields_tags;
}

/// Print help messages on program usage.
void SAMAlignmentCounterArguments::helpMessage()
{
    std::cerr << "Usage: " << prog_name << " [Input SAM File] [Output SAM File] [Parse Header Line] [Parse Header Fields] [Parse Header Fields Attribs] [Parse Alignment Line] [Parse Mandatory Alignment Fields] [Parse Optional Alignment Fields] [Parse Optional Alignment Fields Attribs] [Use Preferred Optional Fields] [Line Delimiter Type of SAM File]" << '\n';
    std::cerr << "       " << "[Input SAM File]: an input SAM file reported by featureCounts from STAR's alignment results." << '\n';
    std::cerr << "       " << "[Output SAM File]: an output SAM file containing unique sequence alignments tagged with unique UMI barcodes. " << '\n';
    std::cerr << "       " << "[Parse Header Line]: indicator for parsing header line (Default: false)." << '\n';
    std::cerr << "       " << "[Parse Header Fields]: indicator for parsing the top structure of each field of header line (Default: false)." << '\n';
    std::cerr << "       " << "[Parse Header Fields Attribs]: indicator for parsing the tag and value attributes of each field of header line (Default: false)." << '\n';
    std::cerr << "       " << "[Parse Alignment Line]: indicator for parsing the top structure of alignment line (Default: true)." << '\n';
    std::cerr << "       " << "[Parse Mandatory Alignment Fields]: indicator for parsing all mandatory fields of alignment line according to the SAM standard (Default: false)." << '\n';
    std::cerr << "       " << "[Parse Optional Alignment Fields]: indicator for parsing the top structure of each optional field of alignment line (Default: true)." << '\n';
    std::cerr << "       " << "[Parse Optional Alignment Fields Attribs]: indicator for parsing the tag, type, and value attributes of each optional field of alignment line (Default: false)." << '\n';
    std::cerr << "       " << "[Use Preferred Optional Fields]: indicator for using a list of preferred optional fields (Default: true)." << '\n';
    std::cerr << "       " << "[Line Delimiter Type of SAM File]: type of line delimiter of input SAM file: unix or windows (Default: unix)." << std::endl;
}
