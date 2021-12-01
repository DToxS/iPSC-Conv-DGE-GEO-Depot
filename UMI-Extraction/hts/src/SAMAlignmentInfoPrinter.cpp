//
//  SAMAlignmentInfoPrinter.cpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 4/4/18.
//  Copyright © 2018 Yuguang Xiong. All rights reserved.
//

#include <string>
#include <iostream>
#include <hts/SAMAlignmentInfoPrinter.hpp>
#include <hts/CompositedDGEIlluminaFASTQSequence.hpp>

namespace hts
{

SAMAlignmentInfoPrinter::SAMAlignmentInfoPrinter() : SAMAlignmentCounterInst() {}

SAMAlignmentInfoPrinter::~SAMAlignmentInfoPrinter() noexcept {}

bool SAMAlignmentInfoPrinter::countHeaderDataLine(const SAMHeaderDataLine& header_data_line, bool& aux_count)
{
    std::cout << '\n';
    std::cout << "SAM header data line" << '\n';

    // Print mandatory fields.
    std::cout << '\n';
    std::cout << "Record type: " << header_data_line.getRecordType() << '\n';
    const auto& header_data_mand_fields = header_data_line.getMandatoryFields();
    std::cout << '\n';
    std::cout << "Mandatory fields" << '\n';
    std::cout << '\t';
    for(const auto& header_data_mand_field : header_data_mand_fields)
    {
        std::cout << "Tag:" << header_data_mand_field.getTag() << ';';
        std::cout << "Value:" << header_data_mand_field.getValue() << '\t';
    }
    std::cout << '\n';

    // Print optional fields.
    const auto& header_data_opt_fields = header_data_line.getOptionalFields();
    if(header_data_opt_fields.size() > 0)
    {
        std::cout << '\n';
        std::cout << "Optional fields" << '\n';
        std::cout << '\t';
        for(const auto& header_data_opt_field : header_data_opt_fields)
        {
            std::cout << "Tag:" << header_data_opt_field.getTag() << ';';
            std::cout << "Value:" << header_data_opt_field.getValue() << '\t';
        }
        std::cout << '\n';
    }

    // Disable SAMAlignmentPipe's printing function.
    return false;
}

bool SAMAlignmentInfoPrinter::countHeaderCommentLine(const SAMHeaderCommentLine& header_comment_line, bool& aux_count)
{
    std::cout << '\n';
    std::cout << "SAM header comment line" << '\n';

    // Print mandatory fields.
    std::cout << '\n';
    std::cout << "Record type: " << header_comment_line.getRecordType() << '\n';
    std::cout << '\n';
    std::cout << "Comment: " << header_comment_line.getCommentField() << '\n';
    if(header_comment_line.getCommentField().front() == '\t') std::cout << "First TAB" << '\n';

    // Disable SAMAlignmentPipe's printing function.
    return false;
}

bool SAMAlignmentInfoPrinter::countAlignmentLine(const SAMCompositedDGEIlluminaSTARFeatureCountsAlignmentLine& alignment_line, bool& aux_count)
{
    std::cout << '\n';
    std::cout << "SAM alignment line" << '\n';

    // Print mandatory fields.
    const auto& align_mand_fields = alignment_line.getMandatoryFields();
    std::cout << '\n';
    std::cout << "Mandatory fields" << '\n';
    std::cout << '\t';
    std::cout << "QNAME: " << align_mand_fields.getQName() << '\n';
    // Extract sequence id information.
    CompositedDGEIlluminaFASTQSequence compos_dge_seq('@'+align_mand_fields.getQName(), align_mand_fields.getSeq(), "+", align_mand_fields.getQual(), true, true);
    std::cout << '\t';
    std::cout << "InstrumentID:" << compos_dge_seq.getInstrumentId() << '\t';
    std::cout << "RunNumber:" << compos_dge_seq.getRunNumber() << '\t';
    std::cout << "FlowcellId:" << compos_dge_seq.getFlowcellId() << '\t';
    std::cout << "LaneNumber:" << compos_dge_seq.getLaneNumber() << '\t';
    std::cout << "TileNumber:" << compos_dge_seq.getTileNumber() << '\t';
    std::cout << "XPos:" << compos_dge_seq.getXPos() << '\t';
    std::cout << "YPos:" << compos_dge_seq.getYPos() << '\t';
    std::cout << "WellBarcode:" << compos_dge_seq.getWellBarcode() << '\t';
    std::cout << "UMIBarcode:" << compos_dge_seq.getUMIBarcode() << '\n';
    // Other mandatory fields.
    std::cout << '\t';
    std::cout << "FLAG: " << align_mand_fields.getFlag() << '\t';
    std::cout << "RNAME: " << align_mand_fields.getRName() << '\t';
    std::cout << "POS: " << align_mand_fields.getPos() << '\t';
    std::cout << "MAPQ: " << align_mand_fields.getMapQ() << '\t';
    std::cout << "CIGAR: " << align_mand_fields.getCigar() << '\t';
    std::cout << "RNEXT: " << align_mand_fields.getRNext() << '\t';
    std::cout << "TLEN: " << align_mand_fields.getTLen() << '\t';
    std::cout << "SEQ: " << align_mand_fields.getSeq() << '\t';
    std::cout << "QUAL: " << align_mand_fields.getQual() << '\n';

    // Print optional fields.
    const auto& align_opt_fields = alignment_line.getOptionalFields();
    if(align_opt_fields.size() > 0)
    {
        std::cout << '\n';
        std::cout << "Optional fields" << '\n';
        std::cout << '\t';
        for(const auto& align_opt_field : align_opt_fields)
        {
            std::cout << "Tag:" << align_opt_field.getTag() << ';';
            std::cout << "Type:" << align_opt_field.getType() << ';';
            std::cout << "Value:" << align_opt_field.getValue() << '\t';
        }
        std::cout << '\n';
    }

    // Disable SAMAlignmentPipe's printing function.
    return false;
}

}
