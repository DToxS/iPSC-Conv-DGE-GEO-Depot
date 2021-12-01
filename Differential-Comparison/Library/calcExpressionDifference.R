# Calculate differentially expressed molecules from their read counts data,
# such as genes and proteins.

calcExpressionDifference <- function(read_counts, exprt_design, drug_groups, bio_spec, time_length, results_dir, subset_field_name=NULL, batch_field_name=NULL, ctrl_state_name="CTRL", use_design_matrix=FALSE, use_glm_ql=FALSE, fdrs=c(0.05), min_samples=3, min_samples_req="all", dispersion="auto", n_top=100, deg_only=TRUE, norm_base=1e+6, remove_small=TRUE, read_counts_title="ReadCounts", deg_title="DEG", top_title="TOP", exp_configs_title="ExpConfigs", calc_title="Calc", plot_title="Plots", deg_image_type="png", deg_image_width=NULL, deg_image_height=NULL, single_deg_image_file=FALSE, plot_bcv=TRUE, plot_smear=TRUE, pt_chars=c(16,19), verbose1=FALSE, verbose2=FALSE, func_dir=NULL)
{
    # subset_field_name: The names of multiple subsets for DEG comparison, such that
    #                       each DEG comparison between two conditions is divided into
    #                       multiple comparisons, each of which corresponds to a subset
    #                       of samples.

    # Load user-defined functions.
    if(is.null(func_dir)) func_dir <- getwd()
    source(file.path(func_dir, "compareGeneExpDiffMulti.R"), local=TRUE)
    source(file.path(func_dir, "recordGeneExpDiffMulti.R"), local=TRUE)
    source(file.path(func_dir, "reportDEGSummary.R"), local=TRUE)

    # Check input arguments.

    # Check subset_field_name.
    if(!is.null(subset_field_name) && !all(subset_field_name %in% colnames(exprt_design)))
    {
        warning("subset_field_name must be one of the column names of experiment design table!")
        return(NULL)
    }

    # Check batch_field_name.
    if(!is.null(batch_field_name) && !all(batch_field_name %in% colnames(exprt_design)))
    {
        warning("batch_field_name must be one of the column names of experiment design table!")
        return(NULL)
    }

    # Field names for differential comparison statistics defined in compareGeneExpDiffSubset.
    diff_stats_field_names <- c("logFC", "logCPM", "PValue", "FDR", "Regulation")

    # Perform differential expression comparison for cell samples treated with
    # multiple drugs in multiple cell lines.

    # Initialize overall summary variables for DEG statistics tables.
    if(!is.null(subset_field_name))
    {
        # The name of subset coverage field for DEG and TOP genes.
        subset_coverage_field_name <- "Coverage"
        # The summary of union DEGs.
        deg_summaries_union <- NULL
        # The summary of intersect DEGs.
        deg_summaries_intersect <- NULL
    }
    else
    {
        subset_coverage_field_name <- NULL
        deg_summaries <- NULL
    }

    # FDRs for calculating DEGs: 0.05 and 0.1.
    for(fdr in fdrs)
    {
        if(verbose1) print(paste("FDR:", fdr))

        # Data directory for the results at current FDR.
        fdr_dir_name <- paste("FDR", fdr, sep="-")
        fdr_dir <- file.path(results_dir, fdr_dir_name)
        write_file <- !all(c(is.null(read_counts_title), is.null(deg_title), is.null(top_title), is.null(exp_configs_title), is.null(calc_title), is.null(plot_title)))
        if(write_file && !dir.exists(fdr_dir)) dir.create(path=fdr_dir, mode="0755")

        # File names of DEG and TOP statistics.
        if(!is.null(subset_field_name))
        {
            if(!is.null(deg_title)) deg_diff_stats_union_file <- file.path(fdr_dir, paste(paste(deg_title, fdr, sep="-"), "Union", "tsv", sep="."))
            else deg_diff_stats_union_file <- NULL
            if(!is.null(deg_title)) deg_diff_stats_intersect_file <- file.path(fdr_dir, paste(paste(deg_title, fdr, sep="-"), "Intersect", "tsv", sep="."))
            else deg_diff_stats_intersect_file <- NULL
            if(!is.null(top_title)) top_diff_stats_union_file <- file.path(fdr_dir, paste(paste(top_title, n_top, sep="-"), "Union", "tsv", sep="."))
            else top_diff_stats_union_file <- NULL
            if(!is.null(top_title)) top_diff_stats_intersect_file <- file.path(fdr_dir, paste(paste(top_title, n_top, sep="-"), "Intersect", "tsv", sep="."))
            else top_diff_stats_intersect_file <- NULL
        }
        else
        {
            if(!is.null(deg_title)) deg_diff_stats_file <- file.path(fdr_dir, paste(paste(deg_title, fdr, sep="-"), "tsv", sep="."))
            else deg_diff_stats_file <- NULL
            if(!is.null(top_title)) top_diff_stats_file <- file.path(fdr_dir, paste(paste(top_title, n_top, sep="-"), "tsv", sep="."))
            else top_diff_stats_file <- NULL
        }

        # Initialize a list of DEG names for all drug groups.
        # Entire DEG comparison results from compareGeneExpDiffMulti.
        deg_counts_stats_list <- list()
        # Summarized counts_stats and diff_stats for DEGs and top-ranked genes.
        if(!is.null(subset_field_name))
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

        # Iterate through all drug groups.
        # Identify the DEGs by control-drug comparison for all drug groups.
        for(drug_group_name in names(drug_groups))
        {
            # Skip CTRL group if needed.
            if(drug_group_name == ctrl_state_name) next

            if(verbose1) print(paste(drug_group_name, "Group"))

            # Obtain a flag list for the drugs included in current drug group.
            drug_group_flags <- exprt_design$State %in% drug_groups[[drug_group_name]]

            # Initialize a list of DEG names for current drug group.
            deg_counts_stats_list_group <- list()
            if(!is.null(subset_field_name))
            {
                deg_diff_stats_union_group <- NULL
                deg_diff_stats_intersect_group <- NULL
                top_diff_stats_union_group <- NULL
                top_diff_stats_intersect_group <- NULL
            }
            else
            {
                deg_counts_stats_group <- list()
                deg_diff_stats_group <- NULL
                top_counts_stats_group <- list()
                top_diff_stats_group <- NULL
            }

            # Iterate through all cell lines.
            cell_lines <- levels(factor(exprt_design$Cell))
            for(cell_line in cell_lines)
            {
                cell_type <- paste(bio_spec, cell_line, sep="-")
                if(verbose1) print(paste("In", cell_type))

                # Prepare experimental information.
                exprt_info <- c(group=drug_group_name, cell=cell_type)
                cell_line_flags <- (exprt_design$Cell==cell_line)
                ctrl_state_flags <- (exprt_design$State==ctrl_state_name)

                # Initialize a list of DEG names for current cell line.
                deg_counts_stats_list_cell <- list()
                if(!is.null(subset_field_name))
                {
                    deg_diff_stats_union_cell <- NULL
                    deg_diff_stats_intersect_cell <- NULL
                    top_diff_stats_union_cell <- NULL
                    top_diff_stats_intersect_cell <- NULL
                }
                else
                {
                    deg_counts_stats_cell <- list()
                    deg_diff_stats_cell <- NULL
                    top_counts_stats_cell <- list()
                    top_diff_stats_cell <- NULL
                }

                # Iterate through all plates.
                plates <- levels(factor(exprt_design$Plate))
                if(is.numeric(exprt_design$Plate)) plates <- as.numeric(plates)
                for(plate in plates)
                {
                    plate_code <- as.character(plate)
                    plate_name <- paste("Plate", plate, sep="-")
                    plate_flags <- (exprt_design$Plate==plate)

                    ################### Prepare comparison of CTRLs and drugs in current drug group ###################

                    # Three dimensions are used for selecting the drugs to include in
                    # each call to the comparison function compareGeneExpDiffMulti:
                    # drug group, cell line, and plate, so that each DEG comparison is
                    # performed for a pair of treatment conditions within a cell line
                    # and within a plate. Depending on different purpose, DEG comparison
                    # can be carried out across different cell lines and/or plates by
                    # removing cell line and/oro plate dimensions.

                    # The selection flags for current cell line and plate.
                    select_flags <- cell_line_flags & plate_flags
                    # Prepare experiment design table by selecting the control
                    # samples and drug-treated samples in current cell line and plate.
                    exprt_design_ctrl_select <- exprt_design[ctrl_state_flags & select_flags,,drop=FALSE]
                    exprt_design_drugs_select <- exprt_design[drug_group_flags & select_flags,,drop=FALSE]
                    if(nrow(exprt_design_ctrl_select)<min_samples || nrow(exprt_design_drugs_select)<min_samples)
                    {
                        # At least min_samples CTRL sample is needed for comparison in a cell line.
                        if(nrow(exprt_design_ctrl_select)<min_samples) warning(paste("At least", min_samples, ctrl_state_name, "sample is required for cell line", paste0(cell_type, "!")))
                        # At least min_samples number of drug samples are needed for comparison in a cell line.
                        if(nrow(exprt_design_drugs_select)<min_samples) warning(paste("At least", min_samples, "drug samples are required for cell line", paste0(cell_type, "!")))
                        next
                    }
                    # Generate the experiment design table containing CTRL and current drug group.
                    exprt_design_ctrl_drugs_select <- rbind(exprt_design_ctrl_select, exprt_design_drugs_select)

                    # Retrieve subset names from experiment design table if a subset
                    # category is specified.
                    # Note: the name of a subset category must be one of the column
                    # names of experiment design table.
                    if(!is.null(subset_field_name)) subset_names <- exprt_design_ctrl_drugs_select[,subset_field_name]
                    else subset_names <- NULL

                    # Retrieve batch names from experiment design table if a batch
                    # category is specified.
                    # Note: the name of a batch category must be one of the column
                    # names of experiment design table.
                    if(!is.null(batch_field_name)) batch_names <- exprt_design_ctrl_drugs_select[,batch_field_name]
                    else batch_names <- NULL

                    # Prepare state names and read counts.
                    # Generate the state names for comparing CTRL and current drug group.
                    ctrl_drugs_state_names <- exprt_design_ctrl_drugs_select$State
                    # Get the read counts matrix of CTRL and current drug group.
                    read_counts_ctrl_drugs <- read_counts[,exprt_design_ctrl_drugs_select$ID,drop=FALSE]
                    colnames(read_counts_ctrl_drugs) <- exprt_design_ctrl_drugs_select$ID
                    # Generate the pairs of state names to compare.
                    drug_state_names <- unique(ctrl_drugs_state_names[ctrl_drugs_state_names!=ctrl_state_name])
                    ctrl_drug_state_name_pairs <- data.frame(Group1=rep(ctrl_state_name,length(drug_state_names)), Group2=drug_state_names, stringsAsFactors=FALSE)

                    ################### Set up output file names ###################

                    # File names for read counts and DEG plots.
                    cell_time_plate_title <- paste(cell_type, time_length, plate_name, sep="-")
                    # File names of calculation results containing read counts, normalized
                    # read counts, read counts statistics, and DEG statistics.
                    if(!is.null(calc_title)) calc_file <- file.path(fdr_dir, paste(paste(cell_time_plate_title, calc_title, sep="-"), "tsv", sep="."))
                    else calc_file <- NULL
                    # File names of DEG plots.
                    if(!is.null(plot_title)) plots_file <- file.path(fdr_dir, paste(paste(cell_time_plate_title, plot_title, sep="-"), deg_image_type, sep="."))
                    else plots_file <- NULL

                    ################### Compare the difference between two conditions ###################

                    # Calculate the DEGs for the drug treatments included in both current drug group and
                    # experiment design table, at current plate number, cell line, and FDR value.
                    deg_counts_stats_list_plate <- compareGeneExpDiffMulti(read_counts=read_counts_ctrl_drugs, group_names=ctrl_drugs_state_names, group_pairs=ctrl_drug_state_name_pairs, ctrl_state_name=ctrl_state_name, use_design_matrix=use_design_matrix, use_glm_ql=use_glm_ql, subset_names=subset_names, batch_names=batch_names, dispersion=dispersion, min_samples=min_samples, min_samples_req=min_samples_req, fdr=fdr, deg_only=deg_only, norm_base=norm_base, remove_small=remove_small, exprt_info=exprt_info, deg_counts_stats_file=calc_file, deg_plots_file=plots_file, deg_image_width=deg_image_width, deg_image_height=deg_image_height, single_deg_image_file=single_deg_image_file, plot_bcv=plot_bcv, plot_smear=plot_smear, pt_chars=pt_chars, verbose=(if(verbose1) verbose2 else verbose1), func_dir=func_dir)

                    if(!is.null(deg_counts_stats_list_plate))
                    {
                        # Save the DEG statistics of current plate of current cell line of current drug group.
                        deg_counts_stats_list_cell[[plate_code]] <- deg_counts_stats_list_plate

                        ################### Generate DEG and TOP statistics ###################

                        # Prepare file names.
                        if(!is.null(read_counts_title)) read_counts_file <- file.path(fdr_dir, paste(paste(cell_time_plate_title, read_counts_title, sep="-"), "tsv", sep="."))
                        else read_counts_file <- NULL
                        if(!is.null(exp_configs_title)) exp_configs_file <- file.path(fdr_dir, paste(paste(cell_time_plate_title, exp_configs_title, sep="-"), "tsv", sep="."))
                        else exp_configs_file <- NULL
                        if(!is.null(top_title)) top_stats_file <- file.path(fdr_dir, paste(paste(cell_time_plate_title, paste(top_title, n_top, sep="-"), sep="-"), "tsv", sep="."))
                        else top_stats_file <- NULL
                        if(!is.null(deg_title)) deg_stats_file <- file.path(fdr_dir, paste(paste(cell_time_plate_title, paste(deg_title, fdr, sep="-"), sep="-"), "tsv", sep="."))
                        else deg_stats_file <- NULL

                        # Record DEG and TOP statistics from comparisons between mulitple conditions.
                        deg_counts_stats_summary_list <- recordGeneExpDiffMulti(deg_counts_stats=deg_counts_stats_list_plate, exprt_design=exprt_design_ctrl_drugs_select, read_counts=read_counts_ctrl_drugs, group_names=ctrl_drugs_state_names, cell_line=cell_line, plate=plate, n_top=n_top, subset_names=subset_names, subset_field_name=subset_field_name, subset_coverage_field_name=subset_coverage_field_name, diff_stats_field_names=diff_stats_field_names, read_counts_file=read_counts_file, exp_configs_file=exp_configs_file, top_stats_file=top_stats_file, deg_stats_file=deg_stats_file, verbose=verbose1, func_dir=func_dir)
                        if(!is.null(deg_counts_stats_summary_list))
                        {
                            # Save the DEG comparison summary tables that contain the comparison
                            # statistics from multiple pair of ctrl-drug comparisons in current
                            # plate, cell line and drug group.
                            if(!is.null(subset_field_name))
                            {
                                deg_diff_stats_union_plate <- deg_counts_stats_summary_list$deg_diff_stats_union
                                deg_diff_stats_union_cell <- rbind(deg_diff_stats_union_cell, deg_diff_stats_union_plate)
                                deg_diff_stats_intersect_plate <- deg_counts_stats_summary_list$deg_diff_stats_intersect
                                deg_diff_stats_intersect_cell <- rbind(deg_diff_stats_intersect_cell, deg_diff_stats_intersect_plate)
                                top_diff_stats_union_plate <- deg_counts_stats_summary_list$top_diff_stats_union
                                top_diff_stats_union_cell <- rbind(top_diff_stats_union_cell, top_diff_stats_union_plate)
                                top_diff_stats_intersect_plate <- deg_counts_stats_summary_list$top_diff_stats_intersect
                                top_diff_stats_intersect_cell <- rbind(top_diff_stats_intersect_cell, top_diff_stats_intersect_plate)

                                ################### Print DEG and TOP summary ###################

                                # Print the number of unique DEGs found for current plate of current cell line of current group.
                                if(verbose1)
                                {
                                    # Union of DEGs from multiple pair of ctrl-drug comparisons.
                                    if(!is.null(deg_diff_stats_union_plate))
                                    {
                                        n_genes_text <- if(nrow(deg_diff_stats_union_plate)>0) paste("A union of",nrow(deg_diff_stats_union_plate)) else "No union"
                                        print(paste(n_genes_text, "DEGs are found in", cell_type, "treated with", paste0(drug_state_names,collapse="/")))
                                    }
                                    else print(paste("Information of union DEGs is not available in", cell_type, "treated with", paste0(drug_state_names,collapse="/")))
                                    # Intersect of DEGs from multiple pair of ctrl-drug comparisons.
                                    if(!is.null(deg_diff_stats_intersect_plate))
                                    {
                                        n_genes_text <- if(nrow(deg_diff_stats_intersect_plate)>0) paste("An intersect of",nrow(deg_diff_stats_intersect_plate)) else "No intersect"
                                        print(paste(n_genes_text, "DEGs are found in", cell_type, "treated with", paste0(drug_state_names,collapse="/")))
                                    }
                                    else print(paste("Information of intersect DEGs is not available in", cell_type, "treated with", paste0(drug_state_names,collapse="/")))
                                    # Union of TOPs from multiple pair of ctrl-drug comparisons.
                                    if(!is.null(top_diff_stats_union_plate))
                                    {
                                        n_genes_text <- if(nrow(top_diff_stats_union_plate)>0) paste("A union of",nrow(top_diff_stats_union_plate)) else "No union"
                                        print(paste(n_genes_text, "TOPs are found in", cell_type, "treated with", paste0(drug_state_names,collapse="/")))
                                    }
                                    else print(paste("Information of union TOPs is not available in", cell_type, "treated with", paste0(drug_state_names,collapse="/")))
                                    # Intersect of DEGs from multiple pair of ctrl-drug comparisons.
                                    if(!is.null(top_diff_stats_intersect_plate))
                                    {
                                        n_genes_text <- if(nrow(top_diff_stats_intersect_plate)>0) paste("An intersect of",nrow(top_diff_stats_intersect_plate)) else "No intersect"
                                        print(paste(n_genes_text, "TOPs are found in", cell_type, "treated with", paste0(drug_state_names,collapse="/")))
                                    }
                                    else print(paste("Information of intersect TOPs is not available in", cell_type, "treated with", paste0(drug_state_names,collapse="/")))
                                }
                            }
                            else
                            {
                                deg_counts_stats_cell[[plate_code]] <- deg_counts_stats_summary_list$deg_counts_stats
                                top_counts_stats_cell[[plate_code]] <- deg_counts_stats_summary_list$top_counts_stats
                                # Save the DEG names of current plate of current cell line of current group.
                                deg_diff_stats_plate <- deg_counts_stats_summary_list$deg_diff_stats
                                deg_diff_stats_cell <- rbind(deg_diff_stats_cell, deg_diff_stats_plate)
                                # Save the TOP names of current plate of current cell line of current group.
                                top_diff_stats_plate <- deg_counts_stats_summary_list$top_diff_stats
                                top_diff_stats_cell <- rbind(top_diff_stats_cell, top_diff_stats_plate)

                                ################### Print DEG and TOP summary ###################

                                # Print the number of unique DEGs found for current plate of current cell line of current group.
                                if(verbose1)
                                {
                                    if(!is.null(deg_diff_stats_plate))
                                    {
                                        n_genes_text <- if(nrow(deg_diff_stats_plate)>0) nrow(deg_diff_stats_plate) else "No"
                                        print(paste(n_genes_text, "DEGs are found in", cell_type, "treated with", paste0(drug_state_names,collapse="/")))
                                    }
                                    else print(paste("Information of DEGs is not available in", cell_type, "treated with", paste0(drug_state_names,collapse="/")))
                                }
                            }
                        }
                        else warning("Empty list of DEG comparison summaries!")
                    }
                    else warning("Empty list of DEG comparison results!")
                }

                # Save the DEG statistics of current cell line of current drug group.
                if(length(deg_counts_stats_list_cell)>0) deg_counts_stats_list_group[[cell_line]] <- deg_counts_stats_list_cell
                # Save summarized DEG and TOP names and statistics of current cell line
                # of current drug group.
                if(!is.null(subset_field_name))
                {
                    # Save summarized DEG names and statistics of current cell line of current drug group.
                    if(!is.null(deg_diff_stats_union_cell)) deg_diff_stats_union_group <- rbind(deg_diff_stats_union_group, deg_diff_stats_union_cell)
                    if(!is.null(deg_diff_stats_intersect_cell)) deg_diff_stats_intersect_group <- rbind(deg_diff_stats_intersect_group, deg_diff_stats_intersect_cell)
                    # Save summarized TOP names and statistics of current cell line of current drug group.
                    if(!is.null(top_diff_stats_union_cell)) top_diff_stats_union_group <- rbind(top_diff_stats_union_group, top_diff_stats_union_cell)
                    if(!is.null(top_diff_stats_intersect_cell)) top_diff_stats_intersect_group <- rbind(top_diff_stats_intersect_group, top_diff_stats_intersect_cell)
                }
                else
                {
                    # Save summarized DEG names and statistics of current cell line of current drug group.
                    if(length(deg_counts_stats_cell)>0) deg_counts_stats_group[[cell_line]] <- deg_counts_stats_cell
                    if(!is.null(deg_diff_stats_cell)) deg_diff_stats_group <- rbind(deg_diff_stats_group, deg_diff_stats_cell)
                    # Save summarized TOP names and statistics of current cell line of current drug group.
                    if(length(top_counts_stats_cell)>0) top_counts_stats_group[[cell_line]] <- top_counts_stats_cell
                    if(!is.null(top_diff_stats_cell)) top_diff_stats_group <- rbind(top_diff_stats_group, top_diff_stats_cell)
                }
            }

            # Save the DEG names and statistics of current drug group.
            if(length(deg_counts_stats_list_group)>0) deg_counts_stats_list[[drug_group_name]] <- deg_counts_stats_list_group
            # Save summarized DEG and TOP names and statistics of current drug group.
            if(!is.null(subset_field_name))
            {
                # Save summarized DEG names and statistics of current drug group.
                if(!is.null(deg_diff_stats_union_group)) deg_diff_stats_union <- rbind(deg_diff_stats_union, deg_diff_stats_union_group)
                if(!is.null(deg_diff_stats_intersect_group)) deg_diff_stats_intersect <- rbind(deg_diff_stats_intersect, deg_diff_stats_intersect_group)
                # Save summarized TOP names and statistics of current drug group.
                if(!is.null(top_diff_stats_union_group)) top_diff_stats_union <- rbind(top_diff_stats_union, top_diff_stats_union_group)
                if(!is.null(top_diff_stats_intersect_group)) top_diff_stats_intersect <- rbind(top_diff_stats_intersect, top_diff_stats_intersect_group)
            }
            else
            {
                # Save summarized DEG names and statistics of current drug group.
                if(length(deg_counts_stats_group)>0) deg_counts_stats[[drug_group_name]] <- deg_counts_stats_group
                if(!is.null(deg_diff_stats_group)) deg_diff_stats <- rbind(deg_diff_stats, deg_diff_stats_group)
                # Save summarized TOP names and statistics of current drug group.
                if(length(top_counts_stats_group)>0) top_counts_stats[[drug_group_name]] <- top_counts_stats_group
                if(!is.null(top_diff_stats_group)) top_diff_stats <- rbind(top_diff_stats, top_diff_stats_group)
            }
        }

        # Save the DEG and TOP names of current drug group.
        if(!is.null(subset_field_name))
        {
            # Save the DEG names of current drug group.
            if(!is.null(deg_diff_stats_union_file))
            {
                # Convert Subset column from list to character strings for printing.
                deg_diff_stats_union_output <- deg_diff_stats_union
                deg_diff_stats_union_output[[subset_field_name]] <- sapply(deg_diff_stats_union_output[[subset_field_name]], function(x){paste0(x,collapse=";")})
                write.table(deg_diff_stats_union_output, deg_diff_stats_union_file, sep="\t", quote=FALSE, row.names=FALSE)
            }
            if(!is.null(deg_diff_stats_intersect_file))
            {
                write.table(deg_diff_stats_intersect, deg_diff_stats_intersect_file, sep="\t", quote=FALSE, row.names=FALSE)
            }
            # Save the TOP names of current drug group.
            if(!is.null(top_diff_stats_union_file))
            {
                # Convert Subset column from list to character strings for printing.
                top_diff_stats_union_output <- top_diff_stats_union
                top_diff_stats_union_output[[subset_field_name]] <- sapply(top_diff_stats_union_output[[subset_field_name]], function(x){paste0(x,collapse=";")})
                write.table(top_diff_stats_union_output, top_diff_stats_union_file, sep="\t", quote=FALSE, row.names=FALSE)
            }
            if(!is.null(top_diff_stats_intersect_file))
            {
                write.table(top_diff_stats_intersect, top_diff_stats_intersect_file, sep="\t", quote=FALSE, row.names=FALSE)
            }
        }
        else
        {
            # Save the DEG names of current drug group.
            if(!is.null(deg_diff_stats_file)) write.table(deg_diff_stats, deg_diff_stats_file, sep="\t", quote=FALSE, row.names=FALSE)
            # Save the TOP names of current drug group.
            if(!is.null(top_diff_stats_file)) write.table(top_diff_stats, top_diff_stats_file, sep="\t", quote=FALSE, row.names=FALSE)
        }

        #################### Summarize DEG statistics ####################

        if(!is.null(subset_field_name))
        {
            if(!is.null(deg_title))
            {
                deg_counts_union_file <- file.path(fdr_dir, paste(paste(deg_title, "Union", fdr, "Summary", sep="-"), "tsv", sep="."))
                deg_counts_intersect_file <- file.path(fdr_dir, paste(paste(deg_title, "Intersect", fdr, "Summary", sep="-"), "tsv", sep="."))
            }
            else
            {
                deg_counts_union_file <- NULL
                deg_counts_intersect_file <- NULL
            }
            # Summarize union DEGs.
            if(verbose1) print("Summary of Union DEGs:")
            deg_summary_union <- reportDEGSummary(deg_diff_stats=deg_diff_stats_union, deg_counts_file=deg_counts_union_file, verbose=verbose1, func_dir=func_dir)
            if(!is.null(deg_summary_union)) deg_summaries_union <- rbind(deg_summaries_union, data.frame(FDR=rep(fdr,nrow(deg_summary_union)), deg_summary_union, stringsAsFactors=FALSE))
            # Summarize intersect DEGs.
            if(verbose1) print("Summary of Intersect DEGs:")
            deg_summary_intersect <- reportDEGSummary(deg_diff_stats=deg_diff_stats_intersect, deg_counts_file=deg_counts_intersect_file, verbose=verbose1, func_dir=func_dir)
            if(!is.null(deg_summary_intersect)) deg_summaries_intersect <- rbind(deg_summaries_intersect, data.frame(FDR=rep(fdr,nrow(deg_summary_intersect)), deg_summary_intersect, stringsAsFactors=FALSE))
        }
        else
        {
            if(!is.null(deg_title)) deg_counts_file <- file.path(fdr_dir, paste(paste(deg_title, fdr, "Summary", sep="-"), "tsv", sep="."))
            else deg_counts_file <- NULL
            # Summarize DEGs.
            if(verbose1) print("DEG Summary:")
            deg_summary <- reportDEGSummary(deg_diff_stats=deg_diff_stats, deg_counts_file=deg_counts_file, verbose=verbose1, func_dir=func_dir)
            if(!is.null(deg_summary)) deg_summaries <- rbind(deg_summaries, data.frame(FDR=rep(fdr,nrow(deg_summary)), deg_summary, stringsAsFactors=FALSE))
        }
    }

    # Save the DEG compairson statistics at all FDR levels.
    if(!is.null(deg_title))
    {
        if(!is.null(subset_field_name) && !is.null(deg_summaries_union))
        {
            if(!is.null(deg_summaries_union))
            {
                deg_summaries_union_file <- file.path(results_dir, paste(paste(deg_title, "Union", "Summary", sep="-"), "tsv", sep="."))
                write.table(deg_summaries_union, deg_summaries_union_file, sep="\t", quote=FALSE, row.names=FALSE)
            }
            if(!is.null(deg_summaries_intersect))
            {
                deg_summaries_intersect_file <- file.path(results_dir, paste(paste(deg_title, "Intersect", "Summary", sep="-"), "tsv", sep="."))
                write.table(deg_summaries_intersect, deg_summaries_intersect_file, sep="\t", quote=FALSE, row.names=FALSE)
            }
        }
        else if(is.null(subset_field_name) && !is.null(deg_summaries))
        {
            deg_summaries_file <- file.path(results_dir, paste(paste(deg_title, "Summary", sep="-"), "tsv", sep="."))
            write.table(deg_summaries, deg_summaries_file, sep="\t", quote=FALSE, row.names=FALSE)
        }
    }

    # Return the list of raw read counts and differential comparison statistics of all genes.
    return(deg_counts_stats_list)
}
