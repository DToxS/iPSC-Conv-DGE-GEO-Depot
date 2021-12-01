//
//  SAMAlignmentCounter.hpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 3/29/18.
//  Copyright © 2018 Granville Xiong. All rights reserved.
//

#ifndef SAMAlignmentCounter_hpp
#define SAMAlignmentCounter_hpp

namespace hts
{

/// \brief Count aligned FASTQ sequences using SAM alignment information.
/// This class checks header data line, header comment line, and alignment line
/// for their output eligibilities in SAMAlignmentPipe.
///
/// \tparam  SAMHeaderDataLineType     The type of SAM header data line.
/// \tparam  SAMHeaderCommentLineType  The type of SAM header comment line.
/// \tparam  SAMAlignmentLineType  The type of SAM alignment line.
template<typename SAMHeaderDataLineType, typename SAMHeaderCommentLineType, typename SAMAlignmentLineType>
class SAMAlignmentCounter
{
public:

    SAMAlignmentCounter() {}

    virtual ~SAMAlignmentCounter() noexcept {}

    /// Check a SAM header data line for output count in SAMAlignmentPipe.
    /// \parame[in]   header_data_line  The header data line to analyze.
    /// \parame[out]  aux_count         An indicator for auxiliary count decision.
    /// \return       A decision of whether or not to count the given line.
    /// Note: this functino needs to be overloaded by derived class.
    virtual bool countHeaderDataLine(const SAMHeaderDataLineType& header_data_line, bool& aux_count)
    {
        return true;
    }

    /// Check a SAM header comment line for output count in SAMAlignmentPipe.
    /// \parame[in]   header_comment_line  The header comment line to analyze.
    /// \parame[out]  aux_count            An indicator for auxiliary count decision.
    /// \return       A decision of whether or not to count the given line.
    /// Note: this functino needs to be overloaded by derived class.
    virtual bool countHeaderCommentLine(const SAMHeaderCommentLineType& header_comment_line, bool& aux_count)
    {
        return true;
    }

    /// Check a SAM alignment line for output count in SAMAlignmentPipe.
    /// \parame[in]   alignment_line  The alignment line to analyze.
    /// \parame[out]  aux_count       An indicator for auxiliary count decision.
    /// \return       A decision of whether or not to count the given line.
    /// Note: this functino needs to be overloaded by derived class.
    virtual bool countAlignmentLine(const SAMAlignmentLineType& alignment_line, bool& aux_count)
    {
        return true;
    }
};

}

#endif /* SAMAlignmentCounter_hpp */
