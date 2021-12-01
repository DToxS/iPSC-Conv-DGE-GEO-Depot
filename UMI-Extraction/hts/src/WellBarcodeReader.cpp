//
//  WellBarcodeReader.cpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 8/8/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#include <hts/WellBarcodeReader.hpp>

namespace hts
{

// Well barcode file provided by the Broad Institute for DGE mRNA sequences
// using the following format:
// 1) 3 data fields: Plate, Well, and Barcode.
// 2) TAB separated.
// 3) No quotes.
// 4) No header line.
// 5) No comments line.
WellBarcodeReader::WellBarcodeReader(const std::string& file_path, const std::string& line_delim_type) : utk::DSVReader(file_path, "\t", false, 3, line_delim_type) {}

WellBarcodeTable WellBarcodeReader::read()
{
    WellBarcodeTable well_barcode_table;
    std::string plate, well, barcode;
    while(readValue(plate, well, barcode)) well_barcode_table[barcode] = well;
    return well_barcode_table;
}

}
