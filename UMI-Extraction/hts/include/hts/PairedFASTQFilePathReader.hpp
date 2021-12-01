//
//  PairedFASTQFilePathReader.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 8/15/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef PairedFASTQFilePathReader_hpp
#define PairedFASTQFilePathReader_hpp

#include <string>
#include <vector>
#include <utility>
#include <utk/DSVReader.hpp>

namespace hts
{

// Paired FASTQ file path.
//
// std::pair Arguments:
//
// First: read 1 of paired-end FASTQ files.
// Second: read 2 of paired-end FASTQ files.
using PairedFASTQFilePath = std::pair<std::string, std::string>;
using PairedFASTQFilePaths = std::vector<PairedFASTQFilePath>;

// The format of paired-end FASTQ file paths:
// 1) a full path of the first read of paired-end FASTQ files;
// 2) a full path of the second read of paired-end FASTQ files;
// 3) Each line contains the paths of read 1 and read 2 FASTQ files separated
//    by TAB.
class PairedFASTQFilePathReader : public utk::DSVReader
{
public:

    PairedFASTQFilePathReader(const std::string& file_path, const std::string& line_delim_type="unix");

    PairedFASTQFilePaths read();
};

}

#endif /* PairedFASTQFilePathReader_hpp */
