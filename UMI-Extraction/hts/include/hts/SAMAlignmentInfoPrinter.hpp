//
//  SAMAlignmentInfoPrinter.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 4/4/18.
//  Copyright © 2018 Yuguang Xiong. All rights reserved.
//

#ifndef SAMAlignmentInfoPrinter_hpp
#define SAMAlignmentInfoPrinter_hpp

#include "SAMAlignmentCounter.hpp"
#include "SAMHeaderDataLine.hpp"
#include "SAMHeaderCommentLine.hpp"
#include "SAMCompositedDGEIlluminaSTARFeatureCountsAlignmentLine.hpp"

namespace hts
{

/// \brief Print various information of SAM alignment file.
class SAMAlignmentInfoPrinter : public SAMAlignmentCounter<SAMHeaderDataLine, SAMHeaderCommentLine, SAMCompositedDGEIlluminaSTARFeatureCountsAlignmentLine>
{
public:

    using SAMAlignmentCounterInst = SAMAlignmentCounter<SAMHeaderDataLine, SAMHeaderCommentLine, SAMCompositedDGEIlluminaSTARFeatureCountsAlignmentLine>;

public:

    SAMAlignmentInfoPrinter();

    virtual ~SAMAlignmentInfoPrinter() noexcept;

    virtual bool countHeaderDataLine(const SAMHeaderDataLine& header_data_line, bool& aux_count) override;

    virtual bool countHeaderCommentLine(const SAMHeaderCommentLine& header_comment_line, bool& aux_count) override;

    virtual bool countAlignmentLine(const SAMCompositedDGEIlluminaSTARFeatureCountsAlignmentLine& alignment_line, bool& aux_count) override;
};

}

#endif /* SAMAlignmentInfoPrinter_hpp */
