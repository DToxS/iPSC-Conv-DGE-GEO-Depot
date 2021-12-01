//
//  PairedFASTQFilePathReader.cpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 8/15/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#include <hts/PairedFASTQFilePathReader.hpp>

namespace hts
{

// The format of paired-end FASTQ file paths:
// 1) a full path of the first read of paired-end FASTQ files;
// 2) a full path of the second read of paired-end FASTQ files;
// 3) Each line contains the paths of read 1 and read 2 FASTQ files separated
//    by TAB.
PairedFASTQFilePathReader::PairedFASTQFilePathReader(const std::string& file_path, const std::string& line_delim_type) : utk::DSVReader(file_path, "\t", true, 0, line_delim_type) {}

PairedFASTQFilePaths PairedFASTQFilePathReader::read()
{
    PairedFASTQFilePaths paired_file_paths;
    std::string file_path_1, file_path_2;
    while(readValue(file_path_1, file_path_2))
    {
        paired_file_paths.push_back(PairedFASTQFilePath(file_path_1, file_path_2));
    }
    return paired_file_paths;
}

}
