//
//  FASTQFileGroupOutputStreams.cpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 8/14/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#include <utk/SystemProperties.hpp>
#include <hts/FASTQFileGroupOutputStreams.hpp>

namespace hts
{

FASTQFileGroupOutputStreams::FASTQFileGroupOutputStreams(const std::string& main_file_name, const std::string& file_dir, const WellBarcodeTable& well_barcode_table)
{
    for(const auto& well_barcode : well_barcode_table)
    {
        std::string file_name = main_file_name + '.' + well_barcode.second + '.' + "fastq";
        std::string file_path = file_dir + utk::FileSystem::path_sep + file_name;
//        operator[](well_barcode.second) = FileStreamType(file_path);
        operator[](well_barcode.second).open(file_path);
    }
}

}
