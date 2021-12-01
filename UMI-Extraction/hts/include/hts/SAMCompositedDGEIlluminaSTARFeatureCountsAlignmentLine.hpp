//
//  SAMCompositedDGEIlluminaSTARFeatureCountsAlignmentLine.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 3/29/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#ifndef SAMCompositedDGEIlluminaSTARFeatureCountsAlignmentLine_hpp
#define SAMCompositedDGEIlluminaSTARFeatureCountsAlignmentLine_hpp

#include <vector>
#include "SAMAlignmentLine.hpp"
#include "SAMCompositedDGEIlluminaAlignmentMandatoryFields.hpp"
#include "SAMAlignmentOptionalField.hpp"
#include "SAMSTARFeatureCountsAlignmentOptionalFields.hpp"

namespace hts
{

/// SAM alignment line of composite 3'-DGE Illumina FASTQ sequence, aligned by
/// STAR and reported by featureCounts program.
using SAMCompositedDGEIlluminaSTARFeatureCountsAlignmentLine = SAMAlignmentLine<SAMCompositedDGEIlluminaAlignmentMandatoryFields, SAMSTARFeatureCountsAlignmentOptionalFields, SAMAlignmentOptionalField>;
using SAMCompositedDGEIlluminaSTARFeatureCountsAlignmentLines = std::vector<SAMCompositedDGEIlluminaSTARFeatureCountsAlignmentLine>;

}

#endif /* SAMCompositedDGEIlluminaSTARFeatureCountsAlignmentLine_hpp */
