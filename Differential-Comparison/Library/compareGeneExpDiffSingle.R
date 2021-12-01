# Perform a DEG comparison for a drug treatment at current plate number, cell line, and FDR value.

compareGeneExpDiffSingle <- function(read_counts, group_names, group_pair, ctrl_state_name="CTRL", use_design_matrix=FALSE, use_glm_ql=FALSE, subset_names=NULL, batch_names=NULL, dispersion="auto", min_samples=3, min_samples_req="all", fdr=0.05, deg_only=TRUE, normalize=TRUE, norm_base=1e+6, remove_small=TRUE, exprt_info=NULL, deg_counts_stats_file=NULL, deg_plots_file=NULL, deg_image_width=NULL, deg_image_height=NULL, single_deg_image_file=FALSE, plot_bcv=FALSE, plot_smear=FALSE, pt_chars=c(16,19), r_compat=TRUE, verbose=FALSE, func_dir=NULL)
{
    # subset_names: A vector of the names of multiple subsets of samples for DEG comparison,
    #               such that each DEG comparison between two groups is divided into multiple
    #               comparisons, each of which corresponds to a subset.
    #               Default: NULL
    # batch_names:  A vector of the names accounting for the batch effects inherited in samples
    #               for DEG comparison, such that the batch variable will be included in a design
    #               matrix which then will be passed to dispersion estimation and differential
    #               comparison based on generalized linear model.
    #               Default: NULL

    # Load user-defined functions.
    if(is.null(func_dir)) func_dir <- getwd()
    source(file.path(func_dir, "compareGeneExpDiffSubset.R"), local=TRUE)

    # Check input arguments.

    # Check the read-counts matrix.
    if(!(is.numeric(read_counts) && is.matrix(read_counts) && nrow(read_counts)>0 && ncol(read_counts)>1))
    {
        warning("read_counts must be a non-empty numerical matrix with two sample columns at least!")
        return(NULL)
    }

    # Check read_counts and group_names.
    if(length(group_names) != ncol(read_counts))
    {
        warning("The length of group_names must be equal to the number of read_counts columns!")
        return(NULL)
    }

    # Check group pair.
    if(!(is.character(group_pair) && is.vector(group_pair) && length(group_pair)==2))
    {
        warning("group_pair must be a character vector of length 2!")
        return(NULL)
    }
    if(length(unique(group_pair)) < length(group_pair))
    {
        warning("group_pair must contain different group names!")
        return(NULL)
    }

    # Check subset_names.
    if(!is.null(subset_names))
    {
        if(is.vector(subset_names))
        {
            if(length(subset_names) != ncol(read_counts))
            {
                warning("The length of subset_names must be equal to the number of read_counts columns!")
                return(NULL)
            }
        }
        else
        {
            warning("subset_names must a vector!")
            return(NULL)
        }
    }

    # Check batch_names.
    if(!is.null(batch_names))
    {
        if(is.vector(batch_names))
        {
            if(length(batch_names) != ncol(read_counts))
            {
                warning("The length of batch_names must be equal to the number of read_counts columns!")
                return(NULL)
            }
        }
        else
        {
            warning("batch_names must a vector!")
            return(NULL)
        }
    }

    # Perform a DEG analysis between a pair of conditions.
    if(!is.null(subset_names))
    {
        unique_subset_names <- sort(unique(subset_names))
        if(verbose) cat("Input samples are divided into ", length(unique_subset_names), " subsets for DEG compairson", "\n", sep="")

        # Perform a DEG analysis for each subset of samples and combine the DEG results.
        deg_counts_stats <- list()
        for(subset_name in unique_subset_names)
        {
            if(verbose) cat("Subset ", subset_name, ": ", sep="")

            # Extract a subset of samples for DEG comparison.
            subset_indctrs <- (group_names%in%group_pair) & (subset_names==subset_name)
            batch_names_subset <- batch_names[subset_indctrs]
            group_names_subset <- group_names[subset_indctrs]
            if(length(unique(group_names_subset)) == length(group_pair))
            {
                # Extract verified required samples for DEG comparison.
                read_counts_subset <- read_counts[,subset_indctrs,drop=FALSE]
                read_counts_subset <- read_counts_subset[rowSums(is.na(read_counts_subset))==0,,drop=FALSE]

                if(verbose)
                {
                    cat(colnames(read_counts_subset), sep=", ")
                    cat("\n")
                }

                # Perform a DEG comparison for a subset of samples.
                deg_counts_stats_subset <- compareGeneExpDiffSubset(read_counts=read_counts_subset, group_names=group_names_subset, group_pair=group_pair, ctrl_state_name=ctrl_state_name, use_design_matrix=use_design_matrix, use_glm_ql=use_glm_ql, batch_names=batch_names_subset, subset_name=subset_name, dispersion=dispersion, min_samples=min_samples, min_samples_req=min_samples_req, fdr=fdr, deg_only=deg_only, normalize=normalize, norm_base=norm_base, remove_small=remove_small, exprt_info=exprt_info, deg_counts_stats_file=deg_counts_stats_file, deg_plots_file=deg_plots_file, deg_image_width=deg_image_width, deg_image_height=deg_image_height, single_deg_image_file=single_deg_image_file, plot_bcv=plot_bcv, plot_smear=plot_smear, pt_chars=pt_chars, r_compat=r_compat, verbose=verbose, func_dir=func_dir)

                # Save the DEG stats table for current subset.
                if(!is.null(deg_counts_stats_subset)) deg_counts_stats[[as.character(subset_name)]] <- deg_counts_stats_subset
            }
            else warning(paste0("Cannot find required samples to compare for subset ", subset_name, " of ", paste0(group_pair,collapse="-"), "!"))
        }
        if(length(deg_counts_stats) == 0) deg_counts_stats <- NULL
    }
    else
    {
        # Perform a DEG analysis for the entire set of samples.
        deg_counts_stats <- compareGeneExpDiffSubset(read_counts=read_counts, group_names=group_names, group_pair=group_pair, ctrl_state_name=ctrl_state_name, use_design_matrix=use_design_matrix, use_glm_ql=use_glm_ql, batch_names=batch_names, dispersion=dispersion, min_samples=min_samples, min_samples_req=min_samples_req, fdr=fdr, deg_only=deg_only, normalize=normalize, norm_base=norm_base, remove_small=remove_small, exprt_info=exprt_info, deg_counts_stats_file=deg_counts_stats_file, deg_plots_file=deg_plots_file, deg_image_width=deg_image_width, deg_image_height=deg_image_height, single_deg_image_file=single_deg_image_file, plot_bcv=plot_bcv, plot_smear=plot_smear, pt_chars=pt_chars, r_compat=r_compat, verbose=verbose, func_dir=func_dir)
    }

    # Return DEG comparison results:
    # 1) a data.frame table of sample read counts and DEG comparison statistics
    #    if subset_names is NULL.
    # 2) a list of multiple data.frame tables of DEG comparison results, each of
    #    which corresponds to one subset of samples.
    return(deg_counts_stats)
}
