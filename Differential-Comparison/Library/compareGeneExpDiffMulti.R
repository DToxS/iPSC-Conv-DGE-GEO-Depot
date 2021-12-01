# Perform the DEG comparison analysis for all drug treatments included a drug group
# at specified plate number, cell line, and FDR value.

compareGeneExpDiffMulti <- function(read_counts, group_names, group_pairs, ctrl_state_name="CTRL", use_design_matrix=FALSE, use_glm_ql=FALSE, subset_names=NULL, batch_names=NULL, dispersion="auto", min_samples=3, min_samples_req="all", fdr=0.05, deg_only=TRUE, normalize=TRUE, norm_base=1e+6, remove_small=TRUE, exprt_info=NULL, deg_counts_stats_file=NULL, deg_plots_file=NULL, deg_image_width=NULL, deg_image_height=NULL, single_deg_image_file=FALSE, plot_bcv=FALSE, plot_smear=FALSE, pt_chars=c(16,19), r_compat=TRUE, verbose=FALSE, func_dir=NULL)
{
    # group_pairs: Multiple pairs of group names used for DEG comparisons,
    #              in the form of a data.frame table where each row is used
    #              as a pair of group names for comparing the change of gene
    #              expression level from the first to the second group.
    #              e.g. data.frame(Group1=c("Wildtype","Wildtype","Knockout"),
    #              Group2=c("Knockout","Mixed","Mixed"), stringsAsFactors=FALASE)

    # Load required library
    require(tools)
    require(stats)

    # Load user-defined functions.
    if(is.null(func_dir)) func_dir <- getwd()
    source(file.path(func_dir, "compareGeneExpDiffSingle.R"), local=TRUE)

    # Check the read-counts matrix.
    if(!(is.numeric(read_counts) && is.matrix(read_counts) && nrow(read_counts)>0 && ncol(read_counts)>1))
    {
        warning("read_counts must be a non-empty numerical matrix with two sample columns at least!")
        return(NULL)
    }

    # Initialize the list of DEG read counts.
    deg_counts_stats_list <- list()

    # Traverse through all drug treatments included current drug group for DEG comparison.
    for(pair_idx in 1:nrow(group_pairs))
    {
        # Get a pair of group names for a list of sample replicates to compare.
        group_pair <- as.character(group_pairs[pair_idx,])

        # Print comparison information to console.
        if(verbose) print(paste0(group_pair[1], " vs ", group_pair[2], ":"))

        # Prepare the dataset of current sample pair for comparison.
        group_pair_idx <- group_names==as.character(group_pair)[1]|group_names==as.character(group_pair)[2]
        group_names_compare <- group_names[group_pair_idx]
        read_counts_compare <- read_counts[,group_pair_idx,drop=FALSE]
        # Remove the rows containing any NA.
        read_counts_compare <- read_counts_compare[rowSums(is.na(read_counts_compare))==0,,drop=FALSE]
        # Remove the rows containing all zero read counts.
        read_counts_compare <- read_counts_compare[rowSums(read_counts_compare)>0,,drop=FALSE]
        sample_names_compare <- colnames(read_counts_compare)

        # Calculate the DEGs for each drug treatment in current drug group at current plate number,
        # cell line, and FDR value.
        deg_counts_stats <- compareGeneExpDiffSingle(read_counts=read_counts_compare, group_names=group_names_compare, group_pair=group_pair, ctrl_state_name=ctrl_state_name, use_design_matrix=use_design_matrix, use_glm_ql=use_glm_ql, subset_names=subset_names, batch_names=batch_names, dispersion=dispersion, min_samples=min_samples, min_samples_req=min_samples_req, fdr=fdr, deg_only=deg_only, normalize=normalize, norm_base=norm_base, remove_small=remove_small, exprt_info=exprt_info, deg_counts_stats_file=deg_counts_stats_file, deg_plots_file=deg_plots_file, deg_image_width=deg_image_width, deg_image_height=deg_image_height, single_deg_image_file=single_deg_image_file, plot_bcv=plot_bcv, plot_smear=plot_smear, pt_chars=pt_chars, r_compat=r_compat, verbose=verbose, func_dir=func_dir)

        # Save current DEG read counts and statistics to the list
        if(!is.null(deg_counts_stats)) deg_counts_stats_list[[paste(group_pair[1],group_pair[2],sep=".")]] <- deg_counts_stats
        else warning("Empty DEG comparison results!")
    }
    if(length(deg_counts_stats_list) == 0) deg_counts_stats_list <- NULL

    # Return the list of DEG read counts.
    return(deg_counts_stats_list)
}
