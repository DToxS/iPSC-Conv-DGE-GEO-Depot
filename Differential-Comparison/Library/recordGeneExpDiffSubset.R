# Record multiple data files generated from DEG comparison between a single
# pair of conditions.

recordGeneExpDiffSubset <- function(group_pair, deg_counts_stats, exprt_design_state1, read_counts_state1, exprt_design_state2, read_counts_state2, cell_line, plate, n_top, subset_name=NULL, read_counts_file=NULL, exp_configs_file=NULL, top_stats_file=NULL, deg_stats_file=NULL, verbose=FALSE)
{
    # deg_counts_stats:  DEG comparison results calculated by compareGeneExpDiffSubset:
    #                    a data.frame table of sample read counts and DEG comparison statistics
    # subset_name: The name of a subset of samples for DEG comparison.
    #              Default: NULL

    # Load required library
    require(tools)

    # Check the availability of deg_counts_stats.
    if(!is.null(deg_counts_stats))
    {
        if(!is.data.frame(deg_counts_stats))
        {
            warning("deg_counts_stats must be a data.frame!")
            return(NULL)
        }
    }
    else
    {
        warning("deg_counts_stats cannot be NULL!")
        return(NULL)
    }

    # Prepare file names.
    if(!is.null(read_counts_file))
    {
        read_counts_file_base <- basename(read_counts_file)
        read_counts_file_dir <- dirname(read_counts_file)
        read_counts_file_main_name <- file_path_sans_ext(read_counts_file_base)
        read_counts_file_ext_name <- file_ext(read_counts_file_base)
    }
    if(!is.null(exp_configs_file))
    {
        exp_configs_file_base <- basename(exp_configs_file)
        exp_configs_file_dir <- dirname(exp_configs_file)
        exp_configs_file_main_name <- file_path_sans_ext(exp_configs_file_base)
        exp_configs_file_ext_name <- file_ext(exp_configs_file_base)
    }
    if(!is.null(top_stats_file))
    {
        top_stats_file_base <- basename(top_stats_file)
        top_stats_file_dir <- dirname(top_stats_file)
        top_stats_file_main_name <- file_path_sans_ext(top_stats_file_base)
        top_stats_file_ext_name <- file_ext(top_stats_file_base)
    }
    if(!is.null(deg_stats_file))
    {
        deg_stats_file_base <- basename(deg_stats_file)
        deg_stats_file_dir <- dirname(deg_stats_file)
        deg_stats_file_main_name <- file_path_sans_ext(deg_stats_file_base)
        deg_stats_file_ext_name <- file_ext(deg_stats_file_base)
    }

    ######## Data file 1: raw read counts of state 1 ########
    # Prepare the dataset of state 1.
    sample_names_state1_compare <- colnames(read_counts_state1)
    # Add common statistics of raw read counts of state 1.
    read_counts_state1_stats_compare <- cbind(Min=rowMins(read_counts_state1), Max=rowMaxs(read_counts_state1), Median=rowMedians(read_counts_state1), Mean=rowMeans(read_counts_state1), Std=rowSds(read_counts_state1))
    # Save raw read counts of state 1.
    if(!is.null(read_counts_file))
    {
        state1_name <- group_pair[1]
        if(!is.null(subset_name)) state1_name <- paste(state1_name, subset_name, sep=".")
        read_counts_state1_file <- paste(read_counts_file_main_name, state1_name, sep=".")
        read_counts_state1_file <- paste(read_counts_state1_file, read_counts_file_ext_name, sep=".")
        read_counts_state1_file <- file.path(read_counts_file_dir, read_counts_state1_file)
        write.table(cbind(Gene=rownames(read_counts_state1), read_counts_state1, read_counts_state1_stats_compare), file=read_counts_state1_file, sep="\t", quote=FALSE, row.names=FALSE)
    }

    ######## Data file 2: experimental configuraitons of state 1 ########
    # Prepare the dataset of state 1.
    # Save experimental configuraitons of state 1.
    if(!is.null(exp_configs_file))
    {
        state1_name <- group_pair[1]
        if(!is.null(subset_name)) state1_name <- paste(state1_name, subset_name, sep=".")
        exprt_design_state1_file <- paste(exp_configs_file_main_name, state1_name, sep=".")
        exprt_design_state1_file <- paste(exprt_design_state1_file, exp_configs_file_ext_name, sep=".")
        exprt_design_state1_file <- file.path(exp_configs_file_dir, exprt_design_state1_file)
        write.table(exprt_design_state1[,colnames(exprt_design_state1)!="Use"], file=exprt_design_state1_file, sep="\t", quote=FALSE, row.names=FALSE)
    }

    ######## Data file 3: raw read counts of state 2 ########
    # Prepare the dataset of state 2.
    sample_names_state2_compare <- colnames(read_counts_state2)
    # Add common statistics of raw read counts of state 2.
    read_counts_state2_stats_compare <- cbind(Min=rowMins(read_counts_state2), Max=rowMaxs(read_counts_state2), Median=rowMedians(read_counts_state2), Mean=rowMeans(read_counts_state2), Std=rowSds(read_counts_state2))
    # Save raw read counts of state 2.
    if(!is.null(read_counts_file))
    {
        state2_name <- group_pair[2]
        if(!is.null(subset_name)) state2_name <- paste(state2_name, subset_name, sep=".")
        read_counts_state2_file <- paste(read_counts_file_main_name, state2_name, sep=".")
        read_counts_state2_file <- paste(read_counts_state2_file, read_counts_file_ext_name, sep=".")
        read_counts_state2_file <- file.path(read_counts_file_dir, read_counts_state2_file)
        write.table(cbind(Gene=rownames(read_counts_state2), read_counts_state2, read_counts_state2_stats_compare), file=read_counts_state2_file, sep="\t", quote=FALSE, row.names=FALSE)
    }

    ######## Data file 4: experimental configuraitons of state 2 ########
    # Prepare the dataset of state 2.
    # Save experimental configuraitons of state 2.
    if(!is.null(exp_configs_file))
    {
        state2_name <- group_pair[2]
        if(!is.null(subset_name)) state2_name <- paste(state2_name, subset_name, sep=".")
        exprt_design_state2_file <- paste(exp_configs_file_main_name, state2_name, sep=".")
        exprt_design_state2_file <- paste(exprt_design_state2_file, exp_configs_file_ext_name, sep=".")
        exprt_design_state2_file <- file.path(exp_configs_file_dir, exprt_design_state2_file)
        write.table(exprt_design_state2[,colnames(exprt_design_state2)!="Use"], file=exprt_design_state2_file, sep="\t", quote=FALSE, row.names=FALSE)
    }

    ######## Data file 5: filtered and normalized read counts of both states ########
    # Prepare the dataset of filtered and normalized read counts.
    sample_names_state1_norm_compare <- paste(sample_names_state1_compare, "Norm", sep=".")
    sample_names_state2_norm_compare <- paste(sample_names_state2_compare, "Norm", sep=".")
    sample_names_norm_compare <- c(sample_names_state1_compare, sample_names_state2_compare, sample_names_state1_norm_compare, sample_names_state2_norm_compare)
    read_counts_norm_compare <- deg_counts_stats[,sample_names_norm_compare]
    # Save raw and normalized read counts.
    if(!is.null(read_counts_file))
    {
        group_pair_name <- paste(group_pair[1],group_pair[2],sep="-")
        if(!is.null(subset_name)) group_pair_name <- paste(group_pair_name, subset_name, sep=".")
        read_counts_norm_file <- paste(read_counts_file_main_name, "Norm", sep="-")
        read_counts_norm_file <- paste(read_counts_norm_file, group_pair_name, sep=".")
        read_counts_norm_file <- paste(read_counts_norm_file, read_counts_file_ext_name, sep=".")
        read_counts_norm_file <- file.path(read_counts_file_dir, read_counts_norm_file)
        write.table(cbind(Gene=rownames(read_counts_norm_compare), read_counts_norm_compare), file=read_counts_norm_file, sep="\t", quote=FALSE, row.names=FALSE)
    }

    ######## Data file 6: Statistics of TOP (top-ranked) genes ########
    # Sort genes according to their p-values.
    deg_counts_stats_group_pair_sorted <- deg_counts_stats[sort.list(deg_counts_stats[,"PValue"], decreasing=FALSE),,drop=FALSE]
    # Extract the top-n genes from the sorted gene list.
    top_counts_stats <- deg_counts_stats_group_pair_sorted[c(1:n_top),,drop=FALSE]
    # Remove FDR and Regulation columns.
    field_names <- colnames(top_counts_stats)
    field_names <- field_names[field_names!="FDR"&field_names!="Regulation"]
    top_counts_stats <- top_counts_stats[,field_names,drop=FALSE]
    top_names <- rownames(top_counts_stats)
    # Save the DEG statistics of current plate of current cell line of current drug group.
    top_diff_stats <- data.frame(Cell=rep(cell_line,n_top), Plate=rep(plate,n_top), Condition1=rep(group_pair[1],n_top), Condition2=rep(group_pair[2],n_top), Gene=top_names, logFC=top_counts_stats[top_names,"logFC"], logCPM=top_counts_stats[top_names,"logCPM"], PValue=top_counts_stats[top_names,"PValue"], stringsAsFactors=FALSE)
    # Save DEG statistics of top-ranked genes.
    if(!is.null(top_stats_file))
    {
        group_pair_name <- paste(group_pair[1],group_pair[2],sep="-")
        if(!is.null(subset_name)) group_pair_name <- paste(group_pair_name, subset_name, sep=".")
        top_stats_file <- paste(top_stats_file_main_name, group_pair_name, sep=".")
        top_stats_file <- paste(top_stats_file, top_stats_file_ext_name, sep=".")
        top_stats_file <- file.path(top_stats_file_dir, top_stats_file)
        write.table(top_diff_stats, file=top_stats_file, sep="\t", quote=FALSE, row.names=FALSE)
    }

    ######## Data file 7: Statistics of DEGs ########
    # Extract DEG statistics of DEGs.
    # Note: input argument deg_counts_stats is modified.
    deg_counts_stats <- deg_counts_stats[deg_counts_stats$Regulation!=0,,drop=FALSE]
    deg_names <- rownames(deg_counts_stats)
    n_deg <- length(deg_names)
    if(verbose)
    {
        if(n_deg > 0)
        {
            if(!is.null(subset_name)) print(paste(n_deg, "DEGs are found in subset", subset_name, "between", group_pair[1], "and", group_pair[2]))
            else print(paste(n_deg, "DEGs are found between", group_pair[1], "and", group_pair[2]))
        }
        else
        {
            if(!is.null(subset_name)) print(paste("No DEGs are found in subset", subset_name, "between", group_pair[1], "and", group_pair[2]))
            else print(paste("No DEGs are found between", group_pair[1], "and", group_pair[2]))
        }
    }
    # Save the statistics of differential comparison for the DEGs of current
    # pair conditions.
    deg_diff_stats <- data.frame(Cell=rep(cell_line,n_deg), Plate=rep(plate,n_deg), Condition1=rep(group_pair[1],n_deg), Condition2=rep(group_pair[2],n_deg), Gene=deg_names, logFC=deg_counts_stats[deg_names,"logFC"], logCPM=deg_counts_stats[deg_names,"logCPM"], PValue=deg_counts_stats[deg_names,"PValue"], FDR=deg_counts_stats[deg_names,"FDR"], stringsAsFactors=FALSE)
    # Save DEG statistics of DEGs.
    if(!is.null(deg_stats_file))
    {
        group_pair_name <- paste(group_pair[1],group_pair[2],sep="-")
        if(!is.null(subset_name)) group_pair_name <- paste(group_pair_name, subset_name, sep=".")
        deg_stats_file <- paste(deg_stats_file_main_name, group_pair_name, sep=".")
        deg_stats_file <- paste(deg_stats_file, deg_stats_file_ext_name, sep=".")
        deg_stats_file <- file.path(deg_stats_file_dir, deg_stats_file)
        write.table(deg_diff_stats, file=deg_stats_file, sep="\t", quote=FALSE, row.names=FALSE)
    }

    # Combine counts_stats and diff_stats from top-ranked genes and DEGs from:
    # four data.frame datasets: deg_counts_stats, deg_diff_stats: a data.frame,
    # top_counts_stats, and top_diff_stats.
    deg_counts_stats_summary <- list(deg_counts_stats=deg_counts_stats, deg_diff_stats=deg_diff_stats, top_counts_stats=top_counts_stats, top_diff_stats=top_diff_stats)

    # Return the DEG comparison summary
    return(deg_counts_stats_summary)
}
