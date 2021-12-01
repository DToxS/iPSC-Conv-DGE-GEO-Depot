# Record multiple data files generated from DEG comparisons between mulitple
# pairs of conditions.

recordGeneExpDiffMulti <- function(deg_counts_stats_list, exprt_design, read_counts, group_names, cell_line, plate, n_top, subset_names=NULL, subset_field_name=NULL, subset_coverage_field_name=NULL, diff_stats_field_names=NULL, read_counts_file=NULL, exp_configs_file=NULL, top_stats_file=NULL, deg_stats_file=NULL, verbose=FALSE, func_dir=NULL)
{
    # subset_names: A vector of the names of multiple subsets for DEG comparison, such that
    #               each DEG comparison between two groups is divided into multiple comparisons,
    #               each of which corresponds to a subset.
    #               Default: NULL

    # Load user-defined functions.
    if(is.null(func_dir)) func_dir <- getwd()
    source(file.path(func_dir, "recordGeneExpDiffSingle.R"), local=TRUE)

    # Check input arguments.

    # Check the availability of deg_counts_stats_list.
    if(!is.null(deg_counts_stats_list))
    {
        if(is.list(deg_counts_stats_list) && !is.data.frame(deg_counts_stats_list))
        {
            if(length(deg_counts_stats_list) == 0)
            {
                warning("deg_counts_stats_list is an empty list!")
                return(NULL)
            }
            else if(is.null(names(deg_counts_stats_list)))
            {
                warning("deg_counts_stats_list must be a named list!")
                return(NULL)
            }
        }
        else
        {
            warning("deg_counts_stats_list must be a list!")
            return(NULL)
        }
    }
    else
    {
        warning("deg_counts_stats_list is not available!")
        return(NULL)
    }

    # Check if the number of exprt_design rows is equal to the number of read_counts columns.
    if(nrow(exprt_design) != ncol(read_counts))
    {
        warning("The number of exprt_design rows must be equal to the number of read_counts columns!")
        return(NULL)
    }

    # Check if the length of group_names is equal to the number of exprt_design rows.
    if(length(group_names) != nrow(exprt_design))
    {
        warning("The length of group_names must be equal to the number of exprt_design rows!")
        return(NULL)
    }

    # Check if the length of subset_names is equal to that of group_names.
    if(!is.null(subset_names))
    {
        if(length(subset_names) != length(group_names))
        {
            warning("The length of subset_names must be equal to the length of group_names!")
            return(NULL)
        }
    }

    # Initialize top-gene statistics and DEG names.
    if(!is.null(subset_names))
    {
        deg_diff_stats_union <- NULL
        deg_diff_stats_intersect <- NULL
        top_diff_stats_union <- NULL
        top_diff_stats_intersect <- NULL
    }
    else
    {
        deg_counts_stats <- list()
        deg_diff_stats <- NULL
        top_counts_stats <- list()
        top_diff_stats <- NULL
    }

    # Extract DEG statistics from each compared condition pair.
    for(group_pair in names(deg_counts_stats_list))
    {
        # Get the names of two compared conditions.
        group_pair_name <- unlist(strsplit(group_pair, " *\\. *"))

        # Get the read counts and DEG statistics of two compared conditions.
        deg_counts_stats_group_pair <- deg_counts_stats_list[[group_pair]]

        # Prepare the datasets of state 1 and 2.
        # Prepare the dataset of state 1.
        group_pair_state1_flags <- group_names==as.character(group_pair_name)[1]
        exprt_design_state1 <- exprt_design[group_pair_state1_flags,,drop=FALSE]
        read_counts_state1 <- read_counts[,group_pair_state1_flags,drop=FALSE]
        read_counts_state1 <- read_counts_state1[rowSums(is.na(read_counts_state1))==0,,drop=FALSE]
        if(!is.null(subset_names)) subset_names_state1 <- subset_names[group_pair_state1_flags]
        else subset_names_state1 <- NULL
        # Prepare the dataset of state 2.
        group_pair_state2_flags <- group_names==as.character(group_pair_name)[2]
        exprt_design_state2 <- exprt_design[group_pair_state2_flags,,drop=FALSE]
        read_counts_state2 <- read_counts[,group_pair_state2_flags,drop=FALSE]
        read_counts_state2 <- read_counts_state2[rowSums(is.na(read_counts_state2))==0,,drop=FALSE]
        if(!is.null(subset_names)) subset_names_state2 <- subset_names[group_pair_state2_flags]
        else subset_names_state2 <- NULL

        # Record DEG and TOP statistics from comparisons between two conditions.
        deg_counts_stats_summary <- recordGeneExpDiffSingle(group_pair=group_pair_name, deg_counts_stats=deg_counts_stats_group_pair, exprt_design_state1=exprt_design_state1, read_counts_state1=read_counts_state1, exprt_design_state2=exprt_design_state2, read_counts_state2=read_counts_state2, cell_line=cell_line, plate=plate, n_top=n_top, subset_names_state1=subset_names_state1, subset_names_state2=subset_names_state2, subset_field_name=subset_field_name, subset_coverage_field_name=subset_coverage_field_name, diff_stats_field_names=diff_stats_field_names, read_counts_file=read_counts_file, exp_configs_file=exp_configs_file, top_stats_file=top_stats_file, deg_stats_file=deg_stats_file, verbose=verbose)

        # Combine the comparison statistics for both DEGs and top-ranked genes
        # from each pair of ctrl-drug comparison.
        if(!is.null(deg_counts_stats_summary))
        {
            if(!is.null(subset_names))
            {
                # The DEG results summary from multiple subsets of samples.
                deg_diff_stats_union <- rbind(deg_diff_stats_union, deg_counts_stats_summary$Combo$deg_diff_stats_union)
                deg_diff_stats_intersect <- rbind(deg_diff_stats_intersect, deg_counts_stats_summary$Combo$deg_diff_stats_intersect)
                top_diff_stats_union <- rbind(top_diff_stats_union, deg_counts_stats_summary$Combo$top_diff_stats_union)
                top_diff_stats_intersect <- rbind(top_diff_stats_intersect, deg_counts_stats_summary$Combo$top_diff_stats_intersect)
            }
            else
            {
                # The DEG results summary from a single set of samples.
                deg_counts_stats[[group_pair]] <- deg_counts_stats_summary$deg_counts_stats
                deg_diff_stats <- rbind(deg_diff_stats, deg_counts_stats_summary$deg_diff_stats)
                top_counts_stats[[group_pair]] <- deg_counts_stats_summary$top_counts_stats
                top_diff_stats <- rbind(top_diff_stats, deg_counts_stats_summary$top_diff_stats)
            }
        }
        else warning("Empty list of DEG comparison summaries!")
    }
    if(!is.null(subset_names)) deg_counts_stats_summary_list <- list(deg_diff_stats_union=deg_diff_stats_union, deg_diff_stats_intersect=deg_diff_stats_intersect, top_diff_stats_union=top_diff_stats_union, top_diff_stats_intersect=top_diff_stats_intersect)
    else deg_counts_stats_summary_list <- list(deg_counts_stats=deg_counts_stats, deg_diff_stats=deg_diff_stats, top_counts_stats=top_counts_stats, top_diff_stats=top_diff_stats)

    # Return the list of DEG comparison summaries that contain the comparison
    # statistics from multiple pair of ctrl-drug comparisons.
    return(deg_counts_stats_summary_list)
}
