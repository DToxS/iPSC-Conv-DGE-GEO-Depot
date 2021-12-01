//
//  SAMGeneUMIAlignmentCounter.cpp
//  High-Throughput-Sequencing
//
//  Created by Yuguang Xiong on 4/4/18.
//  Copyright © 2018 Yuguang Xiong. All rights reserved.
//

#include <hts/SAMGeneUMIAlignmentCounter.hpp>
#include <hts/CompositedDGEIlluminaFASTQSequence.hpp>

namespace hts
{

SAMGeneUMIAlignmentCounter::SAMGeneUMIAlignmentCounter() : SAMAlignmentCounterInst() {}

SAMGeneUMIAlignmentCounter::~SAMGeneUMIAlignmentCounter() noexcept {}

/// Determine if a sequence is uniquely aligned to a gene and also tagged
/// with distinct UMI barcode among all the sequences aligned to that gene.
/// An auxiliary count is used to indicate an unique alignment.
bool SAMGeneUMIAlignmentCounter::countAlignmentLine(const SAMCompositedDGEIlluminaSTARFeatureCountsAlignmentLine& alignment_line, bool& aux_count)
{
    // Initialize the status to false.
    bool status = false;

    // Assuming the SAM file only includes uniquely aligned genes, retrieve
    // the gene and the UMI barcode.
    const auto& align_opt_fields = alignment_line.getOptionalFields();
    if(std::size_t n_target_features = 0; align_opt_fields.getNumberOfTargetFeatures(n_target_features))
    {
        // Only retrieve uniquely aligned sequence.
        if(n_target_features == 1)
        {
            if(std::vector<std::string> target_features; align_opt_fields.getTargetFeatures(target_features))
            {
                // Get the uniquely aligned target gene.
                const std::string& target_gene = target_features.front();
                // Get UMI barcode by creating a composite DGE Illumina FASTQ sequence with
                // the minimum overheads.
                CompositedDGEIlluminaFASTQSequence compos_dge_seq(alignment_line.getMandatoryFields().getQName(), "", "", "", true, false);
                std::string umi_barcode = compos_dge_seq.getUMIBarcode();
                // Concatenate gene name and UMI barcode.
                std::string gene_umi_combo = target_gene + umi_barcode;
                // Insert gene-UMI combo tag into the pool.
                auto result = gene_umi_pool.insert(gene_umi_combo);
                // Get the status of insertion:
                // True: the gene-UMI combo is unique and the insertion succeeds.
                // False: the gene-UMI combo is duplicate and the insertion fails.
                status = result.second;
                // Set auxiliary count to true for uniquely aligned sequence.
                aux_count = true;
            }
        }
    }

    // Return eligibility status.
    return status;
}

}
