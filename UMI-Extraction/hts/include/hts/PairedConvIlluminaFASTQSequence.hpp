//
//  PairedConvIlluminaFASTQSequence.hpp
//  High-Throughput-Sequencing
//
//  Created by Granville Xiong on 7/8/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef PairedConvIlluminaFASTQSequence_hpp
#define PairedConvIlluminaFASTQSequence_hpp

// Note: ConvIlluminaFASTQSequence.hpp must be included before
// PairedFASTQSequenceCreator.hpp for the instantiation of the latter.
#include <vector>
#include <ostream>
#include "ConvIlluminaFASTQSequence.hpp"
#include "PairedFASTQSequenceCreator.hpp"

namespace hts
{

using PairedConvIlluminaFASTQSequence = PairedFASTQSequenceCreator<ConvIlluminaFASTQSequence>;
using PairedConvIlluminaFASTQSequences = std::vector<PairedConvIlluminaFASTQSequence>;

}

#endif /* PairedConvIlluminaFASTQSequence_hpp */
