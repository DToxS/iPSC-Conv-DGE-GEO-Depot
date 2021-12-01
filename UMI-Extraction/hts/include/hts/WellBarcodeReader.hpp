//
//  WellBarcodeReader.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 8/8/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef WellBarcodeReader_hpp
#define WellBarcodeReader_hpp

#include <string>
#include <utk/DSVReader.hpp>
#include "WellBarcodeTable.hpp"

namespace hts
{

// Well barcode file provided by the Broad Institute for DGE mRNA sequences
// using the following format:
// 1) 3 data fields: Plate, Well, and Barcode.
// 2) TAB separated.
// 3) No quotes.
// 4) No header line.
// 5) No comments line.
class WellBarcodeReader : public utk::DSVReader
{
public:

    WellBarcodeReader(const std::string& file_path, const std::string& line_delim_type="unix");
    
    WellBarcodeTable read();
};

}

#endif /* WellBarcodeReader_hpp */
