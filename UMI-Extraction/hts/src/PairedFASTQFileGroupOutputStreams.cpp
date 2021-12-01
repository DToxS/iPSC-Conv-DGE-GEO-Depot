//
//  PairedFASTQFileGroupOutputStreams.cpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 8/14/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#include <utk/SystemProperties.hpp>
#include <hts/PairedFASTQFileGroupOutputStreams.hpp>

namespace hts
{

PairedFASTQFileGroupOutputStreams::PairedFASTQFileGroupOutputStreams(const std::string& main_file_name, const std::string& file_dir, const WellBarcodeTable& well_barcode_table)
{
    for(const auto& well_barcode : well_barcode_table)
    {
        std::string r1_file_name = main_file_name + '.' + "R1" + '.' + well_barcode.second + '.' + "fastq";
        std::string r2_file_name = main_file_name + '.' + "R2" + '.' + well_barcode.second + '.' + "fastq";
        std::string r1_file_path = file_dir + utk::FileSystem::path_sep + r1_file_name;
        std::string r2_file_path = file_dir + utk::FileSystem::path_sep + r2_file_name;
        operator[](well_barcode.second) = PairedFileStreamType(FileStreamType(r1_file_path), FileStreamType(r2_file_path));
    }
}

}
