//
//  ConvIlluminaFASTQSequenceDemuxer.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 8/11/17.
//  Copyright © 2017 Yuguang Xiong. All rights reserved.
//

#ifndef ConvIlluminaFASTQSequenceDemuxer_hpp
#define ConvIlluminaFASTQSequenceDemuxer_hpp

#include "PairedFASTQFileGroupOutputStreams.hpp"
#include "PairedConvIlluminaFASTQSequence.hpp"
#include "FASTQSequenceDemuxer.hpp"

namespace hts
{

/// \brief Demultiplexer of Conv FASTQ sequence
/// The demultiplexer of paired Conv FASTQ sequence.
using ConvIlluminaFASTQSequenceDemuxer = FASTQSequenceDemuxer<PairedFASTQFileGroupOutputStreams, PairedConvIlluminaFASTQSequence>;

}

#endif /* ConvIlluminaFASTQSequenceDemuxer_hpp */
