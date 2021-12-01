# Record multiple data files generated from DEG comparison between a single
# pair of conditions for multiple subsets of samples.

recordGeneExpDiffSingle <- function(group_pair, deg_counts_stats, exprt_design_state1, read_counts_state1, exprt_design_state2, read_counts_state2, cell_line, plate, n_top, subset_names_state1=NULL, subset_names_state2=NULL, subset_field_name=NULL, subset_coverage_field_name=NULL, diff_stats_field_names=NULL, read_counts_file=NULL, exp_configs_file=NULL, top_stats_file=NULL, deg_stats_file=NULL, verbose=FALSE)
{
    # deg_counts_stats:  DEG comparison results calculated by compareGeneExpDiffSingle.
    #                    1) a data.frame table of sample read counts and DEG comparison statistics
    #                       if subset_names_state1 and subset_names_state2 are NULL.
    #                    2) a list of multiple data.frame tables of DEG comparison results, each of
    #                       which corresponds to one subset of samples.
    # subset_names_state1: A vector of the names of multiple subsets of samples at condition 1.
    #                      Default: NULL
    # subset_names_state2: A vector of the names of multiple subsets of samples at condition 2.
    #                      Default: NULL
    # subset_field_name: the name of a subset field for recording a list of the subsets where a DEG
    #                    or TOP (top-ranked) gene is generated.
    #                    Default: NULL
    # subset_coverage_field_name: the name of subset coverage field for DEG and TOP genes.
    #                             Default: NULL
    # diff_stats_field_names: the field names for differential comparison statistics, defined in
    #                         compareGeneExpDiffSubset, which needs to be excluded for the output
    #                         deg_diff_stats and top_diff_stats tables as they are summarized from
    #                         multiple subset tables.
    #                         Default: NULL

    # Load required library
    require(tools)

    # Load user-defined functions.
    if(is.null(func_dir)) func_dir <- getwd()
    source(file.path(func_dir, "recordGeneExpDiffSubset.R"), local=TRUE)

    # Check input arguments.

    # Check the availability of deg_counts_stats.
    if(!is.null(deg_counts_stats))
    {
        # When deg_counts_stats is a list.
        if(is.list(deg_counts_stats) && !is.data.frame(deg_counts_stats))
        {
            if(length(deg_counts_stats) == 0)
            {
                warning("deg_counts_stats is an empty list!")
                return(NULL)
            }
            else if(is.null(names(deg_counts_stats)))
            {
                warning("deg_counts_stats must be a named list!")
                return(NULL)
            }
        }
    }
    else
    {
        warning("deg_counts_stats cannot be NULL!")
        return(NULL)
    }

    # Check subset_names_state1 and subset_names_state2.
    if(xor(is.null(subset_names_state1), is.null(subset_names_state2)))
    {
        warning("subset_names_state1 and subset_names_state2 must be NULL or non-NULL at the same time!")
        return(NULL)
    }

    # Check deg_counts_stats, subset_field_name and diff_stats_field_names.
    if(!(is.null(subset_names_state1) && is.null(subset_names_state2)))
    {
        # Check deg_counts_stats.
        if(is.data.frame(deg_counts_stats) || !is.list(deg_counts_stats))
        {
            warning("deg_counts_stats must be a list if subset_names_state1 and/or subset_names_state2 are available!")
            return(NULL)
        }
        # Check subset_field_name.
        if(!(!is.null(subset_field_name) && is.character(subset_field_name) && is.vector(subset_field_name) && length(subset_field_name)==1))
        {
            # The name of a subset field for recording a list of the subsets where a DEG
            # or TOP (top-ranked) gene is generated.
            warning("subset_field_name must be specified for the subset field if subset_names_state1 and/or subset_names_state2 are available!")
            return(NULL)
        }
        # Check subset_coverage_field_name.
        if(!(!is.null(subset_coverage_field_name) && is.character(subset_coverage_field_name) && is.vector(subset_coverage_field_name) && length(subset_coverage_field_name)==1))
        {
            # The name of subset coverage field for DEG and TOP genes.
            warning("subset_coverage_field_name must be specified for the subset field if subset_names_state1 and/or subset_names_state2 are available!")
            return(NULL)
        }
        # Check diff_stats_field_names.
        if(!(!is.null(diff_stats_field_names) && is.character(diff_stats_field_names) && is.vector(diff_stats_field_names) && length(diff_stats_field_names)>0))
        {
            warning("diff_stats_field_names must be specified for differential comparison fields if subset_names_state1 and/or subset_names_state2 are available!")
            return(NULL)
        }
    }

    # Record multiple data files generated from DEG comparison between a single
    # pair of conditions for single set or multiple subsets of samples.
    if(!is.null(subset_names_state1) || !is.null(subset_names_state2))
    {
        # Set the field name for the total number of subsets where DEG
        # comparison is performed.
        last_letter <- substr(subset_field_name, start=nchar(subset_field_name), stop=nchar(subset_field_name))
        if(last_letter=="s" || last_letter=="x") plural_suffix <- "es" else plural_suffix <- "s"
        subsets_field_name <- paste0("Total", subset_field_name, plural_suffix)

        # Initialize summary variables.
        deg_counts_stats_summary_subsets <- list()

        # Union DEG tables contain a field of subset list, in addition to the
        # fields of Cell, Plate, Condition1, Condition2 and Gene obtained from
        # compareGeneExpDiffSubset function.
        # Note: the fields of statistical results, logFC, logCPM, PValue, FDR
        # and Regulation  are removed because these results are different from
        # different subsets of samples.
        deg_diff_stats_union <- deg_diff_stats_intersect <- data.frame()
        top_diff_stats_union <- top_diff_stats_intersect <- data.frame()

        # Summarize DEG comparison results for each subset of samples.
        subset_names <- names(deg_counts_stats)
        n_subset_names <- length(subset_names)
        for(subset_name in subset_names)
        {
            # Retrieve data variables for current sample subset.
            deg_counts_stats_subset <- deg_counts_stats[[subset_name]]
            subset_indctrs_state1 <- subset_names_state1==subset_name
            exprt_design_state1_subset <- exprt_design_state1[subset_indctrs_state1,,drop=FALSE]
            read_counts_state1_subset <- read_counts_state1[,subset_indctrs_state1,drop=FALSE]
            subset_indctrs_state2 <- subset_names_state2==subset_name
            exprt_design_state2_subset <- exprt_design_state2[subset_indctrs_state2,,drop=FALSE]
            read_counts_state2_subset <- read_counts_state2[,subset_indctrs_state2,drop=FALSE]

            # Summarize DEG comparison results.
            deg_counts_stats_summary_subset <- recordGeneExpDiffSubset(group_pair=group_pair, deg_counts_stats=deg_counts_stats_subset, exprt_design_state1=exprt_design_state1_subset, read_counts_state1=read_counts_state1_subset, exprt_design_state2=exprt_design_state2_subset, read_counts_state2=read_counts_state2_subset, cell_line=cell_line, plate=plate, n_top=n_top, subset_name=subset_name, read_counts_file=read_counts_file, exp_configs_file=exp_configs_file, top_stats_file=top_stats_file, deg_stats_file=deg_stats_file, verbose=verbose)

            # Save DEG comparison results summary.
            deg_counts_stats_summary_subsets[[as.character(subset_name)]] <- deg_counts_stats_summary_subset

            # Update the union and intersect DEG tables with the DEGs
            # from current sample subset.
            deg_diff_stats <- deg_counts_stats_summary_subset$deg_diff_stats
            deg_diff_stats_field_names <- colnames(deg_diff_stats)
            deg_diff_stats_field_names <- deg_diff_stats_field_names[!(deg_diff_stats_field_names%in%diff_stats_field_names)]
            if(nrow(deg_diff_stats) > 0)
            {
                # Update the union DEG table with the DEGs from current sample subset.
                if(nrow(deg_diff_stats_union) > 0)
                {
                    # Find new and existing DEGs.
                    new_union_degs <- setdiff(deg_diff_stats$Gene, deg_diff_stats_union$Gene)
                    exist_union_degs <- intersect(deg_diff_stats$Gene, deg_diff_stats_union$Gene)
                    if(length(new_union_degs) > 0)
                    {
                        # The fields in DEG calculation results generated from compareGeneExpDiffSubset,
                        # excluding those statistical fields not used for summarizing over subset results.
                        new_deg_diff_stats_union <- deg_diff_stats[deg_diff_stats$Gene%in%new_union_degs,deg_diff_stats_field_names,drop=FALSE]
                        # The total number of subsets where DEG comparison is performed.
                        new_deg_diff_stats_union[[subsets_field_name]] <- rep(n_subset_names, nrow(new_deg_diff_stats_union))
                        # A list of the subsets where DEG comparison generates current gene.
                        new_deg_diff_stats_union[[subset_field_name]] <- as.list(rep(subset_name, nrow(new_deg_diff_stats_union)))
                        deg_diff_stats_union <- rbind(deg_diff_stats_union, new_deg_diff_stats_union)
                    }
                    # Update the entries of existing DEGs in the union DEG table.
                    if(length(exist_union_degs) > 0)
                    {
                        # Append current subset name to the subsets list of each existing DEGs.
                        exist_deg_flags <- deg_diff_stats_union$Gene%in%exist_union_degs
                        exist_deg_subsets_list <- sapply(deg_diff_stats_union[exist_deg_flags,subset_field_name], function(x){c(x,subset_name)}, simplify=FALSE)
                        deg_diff_stats_union[exist_deg_flags,subset_field_name] <- list(exist_deg_subsets_list)
                    }
                }
                else
                {
                    # Create a table with comparison statistics and a Subsets field with
                    # the DEGs from current sample subset.
                    # The fields in DEG calculation results generated from compareGeneExpDiffSubset,
                    # excluding those statistical fields not used for summarizing over subset results.
                    deg_diff_stats_union <- deg_diff_stats[,deg_diff_stats_field_names]
                    # The total number of subsets where DEG comparison is performed.
                    deg_diff_stats_union[[subsets_field_name]] <- rep(n_subset_names, nrow(deg_diff_stats_union))
                    # A list of the subsets where DEG comparison generates current gene.
                    deg_diff_stats_union[[subset_field_name]] <- as.list(rep(subset_name, nrow(deg_diff_stats_union)))
                }

                # Update the intersect DEG table with the DEGs from current sample subset.
                if(nrow(deg_diff_stats_intersect) > 0)
                {
                    # Compare the DEGs from current subset with the existing intersect DEG table
                    # to identify intersect DEGs.
                    intersect_degs <- intersect(deg_diff_stats$Gene, deg_diff_stats_intersect$Gene)
                    # Update existing intersect DEG table with intersect DEGs.
                    if(length(intersect_degs) > 0) deg_diff_stats_intersect <- deg_diff_stats_intersect[deg_diff_stats_intersect$Gene%in%intersect_degs,,drop=FALSE]
                    else deg_diff_stats_intersect <- deg_diff_stats_intersect[NULL,,drop=FALSE]
                }
                else deg_diff_stats_intersect <- deg_diff_stats[,deg_diff_stats_field_names,drop=FALSE]
            }

            # Update the union and intersect TOP tables with the TOPs
            # from current sample subset.
            top_diff_stats <- deg_counts_stats_summary_subset$top_diff_stats
            top_diff_stats_field_names <- colnames(top_diff_stats)
            top_diff_stats_field_names <- top_diff_stats_field_names[!(top_diff_stats_field_names%in%diff_stats_field_names)]
            if(nrow(top_diff_stats) > 0)
            {
                # Update the union TOP table with the TOPs from current sample subset.
                if(nrow(top_diff_stats_union) > 0)
                {
                    # Find new and existing TOPs.
                    new_union_tops <- setdiff(top_diff_stats$Gene, top_diff_stats_union$Gene)
                    exist_union_tops <- intersect(top_diff_stats$Gene, top_diff_stats_union$Gene)
                    # Add new TOPs to the union TOP table.
                    if(length(new_union_tops) > 0)
                    {
                        # The fields in TOP calculation results generated from compareGeneExpDiffSubset,
                        # excluding those statistical fields not used for summarizing over subset results.
                        # The fields in TOP calculation results generated from compareGeneExpDiffSubset,
                        # excluding those statistical fields not used for summarizing over subset results.
                        new_top_diff_stats_union <- top_diff_stats[top_diff_stats$Gene%in%new_union_tops,top_diff_stats_field_names,drop=FALSE]
                        # The total number of subsets where DEG comparison is performed.
                        new_top_diff_stats_union[[subsets_field_name]] <- rep(n_subset_names, nrow(new_top_diff_stats_union))
                        # A list of the subsets where DEG comparison generates current gene.
                        new_top_diff_stats_union[[subset_field_name]] <- as.list(rep(subset_name, nrow(new_top_diff_stats_union)))
                        top_diff_stats_union <- rbind(top_diff_stats_union, new_top_diff_stats_union)
                    }
                    # Update the entries of existing TOPs in the union TOP table.
                    if(length(exist_union_tops) > 0)
                    {
                        # Append current subset name to the subsets list of each existing TOPs.
                        exist_top_flags <- top_diff_stats_union$Gene%in%exist_union_tops
                        exist_top_subsets_list <- sapply(top_diff_stats_union[exist_top_flags,subset_field_name], function(x){c(x,subset_name)}, simplify=FALSE)
                        top_diff_stats_union[exist_top_flags,subset_field_name] <- list(exist_top_subsets_list)
                    }
                }
                else
                {
                    # Create a table with comparison statistics and a Subsets field with
                    # the TOPs from current sample subset.
                    # The fields in TOP calculation results generated from compareGeneExpDiffSubset,
                    # excluding those statistical fields not used for summarizing over subset results.
                    top_diff_stats_union <- top_diff_stats[,top_diff_stats_field_names]
                    # The total number of subsets where DEG comparison is performed.
                    top_diff_stats_union[[subsets_field_name]] <- rep(n_subset_names, nrow(top_diff_stats_union))
                    # A list of the subsets where DEG comparison generates current gene.
                    top_diff_stats_union[[subset_field_name]] <- as.list(rep(subset_name, nrow(top_diff_stats_union)))
                }

                # Update the intersect TOP table with the TOPs from current sample subset.
                if(nrow(top_diff_stats_intersect) > 0)
                {
                    # Compare the TOPs from current subset with the existing intersect TOP table
                    # to identify intersect TOPs.
                    intersect_tops <- intersect(top_diff_stats$Gene, top_diff_stats_intersect$Gene)
                    # Update existing intersect TOP table with intersect TOPs.
                    if(length(intersect_tops) > 0) top_diff_stats_intersect <- top_diff_stats_intersect[top_diff_stats_intersect$Gene%in%intersect_tops,,drop=FALSE]
                    else top_diff_stats_intersect <- top_diff_stats_intersect[NULL,,drop=FALSE]
                }
                else top_diff_stats_intersect <- top_diff_stats[,top_diff_stats_field_names,drop=FALSE]
            }
        }

        # Calculate and add subset coverage of union DEGs and TOPs:
        # The coverage of a DEG is defined as the ratio between the number of subsets that
        # generate this DEG and the number of all subsets that can generate DEGs.
        # Note: the denominator for coverage calculation is not the number of the total
        # number of subsets, but those that generate DEGs.
        n_deg_subsets <- length(unique(unlist(deg_diff_stats_union[[subset_field_name]])))
        deg_diff_stats_union[[subset_coverage_field_name]] <- sapply(deg_diff_stats_union[[subset_field_name]], function(x){length(x)/n_deg_subsets})
        n_top_subsets <- length(unique(unlist(top_diff_stats_union[[subset_field_name]])))
        top_diff_stats_union[[subset_coverage_field_name]] <- sapply(top_diff_stats_union[[subset_field_name]], function(x){length(x)/n_top_subsets})

        # Save the combined comparison statistics.
        deg_counts_stats_summary_combo <- list(deg_diff_stats_union=deg_diff_stats_union, deg_diff_stats_intersect=deg_diff_stats_intersect, top_diff_stats_union=top_diff_stats_union, top_diff_stats_intersect=top_diff_stats_intersect)

        # Summarize DEG comparison results for multiple subsets of samples.
        deg_counts_stats_summary <- list(Subsets=deg_counts_stats_summary_subsets, Combo=deg_counts_stats_summary_combo)
    }
    else
    {
        # Summarize DEG comparison results for a single set of samples.
        deg_counts_stats_summary <- recordGeneExpDiffSubset(group_pair=group_pair, deg_counts_stats=deg_counts_stats, exprt_design_state1=exprt_design_state1, read_counts_state1=read_counts_state1, exprt_design_state2=exprt_design_state2, read_counts_state2=read_counts_state2, cell_line=cell_line, plate=plate, n_top=n_top, read_counts_file=read_counts_file, exp_configs_file=exp_configs_file, top_stats_file=top_stats_file, deg_stats_file=deg_stats_file, verbose=verbose)
    }

    # Return the list of DEG comparison summaries.
    return(deg_counts_stats_summary)
}
