//
//  ConvIlluminaFASTQFile.hpp
//  High-Throughput-Sequencing
//
//  Created by Granville Xiong on 5/9/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef ConvIlluminaFASTQFile_hpp
#define ConvIlluminaFASTQFile_hpp

#include "ConvIlluminaFASTQSequence.hpp"
#include "FASTQFileReader.hpp"

namespace hts
{

using ConvIlluminaFASTQFile = FASTQFileReader<ConvIlluminaFASTQSequence, bool, bool, bool, bool>;

}

#endif /* ConvIlluminaFASTQFile_hpp */
