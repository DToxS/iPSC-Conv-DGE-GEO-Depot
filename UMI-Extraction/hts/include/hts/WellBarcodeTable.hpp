//
//  WellBarcodeTable.hpp
//  High-Throughput-Sequencing
//
//  Created by Granville Xiong on 8/12/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef WellBarcodeTable_hpp
#define WellBarcodeTable_hpp

#include <string>
#include <map>

namespace hts
{

// An association between well barcodes and corresponding well numbers.
//
// std::map Arguments:
//
// First: well barcode (key) to be matched with the group ID of FASTQ sequences.
// Second: well number (value) for naming corresponding cell sample (each sample
//         has a unique well barcode.
using WellBarcodeTable = std::map<std::string, std::string>;

}

#endif /* WellBarcodeTable_hpp */
