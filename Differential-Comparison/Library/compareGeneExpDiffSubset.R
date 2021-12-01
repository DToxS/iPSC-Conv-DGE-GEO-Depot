# Perform a DEG comparison for a subset of samples under a drug treatment at current plate number,
# cell line, and FDR value.

# Append experiment info to a title text.
appendExprtInfo <- function(title_text, exprt_info)
{
    if(!is.null(title_text) && !is.null(exprt_info))
    {
        if(!is.na(exprt_info["cell"])) title_text <- paste(title_text, "in", exprt_info["cell"])
        if(!is.na(exprt_info["time"])) title_text <- paste(title_text, "at", exprt_info["time"])
    }
    return(title_text)
}

# Perform a DEG comparison for a subset of samples between a pair of conditions.
compareGeneExpDiffSubset <- function(read_counts, group_names, group_pair, ctrl_state_name="CTRL", use_design_matrix=FALSE, use_glm_ql=FALSE, batch_names=NULL, subset_name=NULL, dispersion="auto", min_samples=3, min_samples_req="all", fdr=0.05, deg_only=TRUE, normalize=TRUE, norm_base=1e+6, remove_small=TRUE, exprt_info=NULL, deg_counts_stats_file=NULL, deg_plots_file=NULL, deg_image_width=NULL, deg_image_height=NULL, single_deg_image_file=FALSE, plot_bcv=FALSE, plot_smear=FALSE, pt_chars=c(16,19), r_compat=TRUE, verbose=FALSE, func_dir=NULL)
{
    # read_counts: A matrix of read counts and sequence length of genes at different sample
    #              conditions.
    # group_names: A vector of group names assigned to each sample used for DEG comparison.
    #              e.g. c("Wildtype", "Wildtype", "Knockout", "Knockout", "Mixed", "Mixed")
    # group_pair: A pair of group names used for a comparison of the change of gene expression
    #             level from group_pair[1] to group_pair[2].
    #             e.g. c("Wildtype","Knockout")
    # ctrl_state_name: the name of control state.
    #                  Default: CTRL
    # use_design_matrix: Whether to use a design matrix for differential comparison.
    #                    Default: FALSE
    #                    Note: if batch_names is given, always use design matrix.
    # use_glm_ql: Whether to GLM model with quasi-likelihood test.
    #             Default: FALSE
    # batch_names: A vector of batch names assigned to each sample used for DEG comparison.
    #              e.g. c("Conv", "Conv", "DGE", "DGE", "Conv", "DGE")
    # subset_name: The name of a subset of samples for DEG comparison.
    #              Default: NULL
    # dispersion: The dispersion argument passed to exactTest or GLM fit functions:
    #             Allowed: auto, common, trended, tagwise
    #             Default: auto
    # min_samples: The minimum number of sample replicates in each sample group needed for
    #              outlier removal and DEG comparison.
    #              Default: 3
    # min_samples_req: The requirement for min_samples to be met by sample groups for DEG
    #                  comparison.
    #                  Allowed: all, any
    #                  Default: all
    # fdr: false discovery rate.
    #      Default: 0.05
    # deg_only: Output only the genes whose FDR is smaller than specified threshold.
    #           Default: TRUE
    # normalize: Whether to normalize the read counts of each sample before DEG comparison.
    #            Default: TRUE
    # remove_small: Whether to remove the genes with small read counts before DEG comparison.
    #               Default: TRUE
    # exprt_info: The experiment information to be included in plots.
    #             e.g. drug group, cell line, time point, etc.
    #             Default: NULL
    # deg_counts_stats_file: The name of output file containing detailed DEG comparison results.
    #                       e.g. Human.A-Hour.48-Plate.1-Calc-CTRL.TRS+LOP.tsv
    # deg_plots_file: The name of output DEG plot image file whose extension name can be one of
    #                 png, jpeg/jpg, svg, pdf, eps.
    #                 e.g. Human.A-Hour.48-Plate.1-DEG.0.1-Plots-CTRL.TRS+LOP.pdf
    #                 Default: NULL
    # deg_image_width: The width of output plot image file.
    #                  e.g. 1600 for raster image and 8.5 for vector image.
    #                  Default: NULL
    # deg_image_height: The height of output plot image file.
    #                  e.g. 1600 for raster image and 8.5 for vector image.
    #                  Default: NULL
    # single_deg_image_file: Whether to output multiple plots into a single image file.
    #                        Default: FALSE
    # plot_bcv: Whether to output the plot of biological coefficient of variation (BCV).
    #           Default: FALSE
    # plot_smear: Whether to output Smear plot for mean and average (MA plot).
    #             Default: FALSE
    # pt_chars: The character symbols to use in BCV plot and Smear plot.
    #           Default: c(16,19)
    # r_compat: Whether to output text table file compatible with R.
    #           Default: TRUE
    # verbose: Whether to print detailed information on screen.
    #          Default: FALSE
    # func_dir: The directory where custom functions are loaded.
    #           e.g. /Users/granville/Documents/works/academia/MSSM/LINCS/Programs/
    #           Default: NULL

    # Load required library
    require(tools)
    require(stats)
    require(edgeR)

    # Load user-defined functions.
    if(is.null(func_dir)) func_dir <- getwd()
    source(file.path(func_dir, "openPlotDevice.R"), local=TRUE)
    source(file.path(func_dir, "closePlotDevice.R"), local=TRUE)
    source(file.path(func_dir, "formatDecimal.R"), local=TRUE)
    source(file.path(func_dir, "createDGEList.R"), local=TRUE)
    source(file.path(func_dir, "cpb.R"), local=TRUE)
    source(file.path(func_dir, "cpb.DGEList.R"), local=TRUE)
    source(file.path(func_dir, "cpb.default.R"), local=TRUE)

    # Check input arguments.

    # Check the read-counts matrix.
    if(!(is.numeric(read_counts) && is.matrix(read_counts) && nrow(read_counts)>0 && ncol(read_counts)>1))
    {
        warning("read_counts must be a non-empty numerical matrix with two sample columns at least!")
        return(NULL)
    }

    # Check read_counts and group_names.
    if(ncol(read_counts) != length(group_names))
    {
        warning("The column number of read_counts must be equal to the length of group_names!")
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

    # Check read_counts and batch_names.
    if(!is.null(batch_names))
    {
        if(is.vector(batch_names))
        {
            if(ncol(read_counts) == length(batch_names))
            {
                if(!is.factor(batch_names)) batch_factors <- as.factor(batch_names)
                else batch_factors <- batch_names
            }
            else
            {
                warning("The column number of read_counts must be equal to the length of batch_names!")
                return(NULL)
            }
        }
        else
        {
            warning("batch_names must a vector!")
            return(NULL)
        }
    }
    else batch_factors <- NULL

    # Check ctrl_state_name.
    if(!is.null(batch_factors) || use_design_matrix)
    {
        if(is.null(ctrl_state_name))
        {
            warning("The name of control condition must be specified for the purpose of the reference level used in design matrix!")
            return(NULL)
        }
    }

    # Check the dispersion parameter.
    dispersion_types <- c("common", "trended", "tagwise", "auto")
    if(any(dispersion %in% dispersion_types))
    {
        if(use_design_matrix && use_glm_ql)
        {
            if(dispersion!="common" && dispersion!="trended")
            {
                warning("dispersion must be either common or trended when generalized linear model with quasi-likelihood test is used!")
                return(NULL)
            }
        }
    }
    else
    {
        warning(paste0("dispersion must be a character string equal to one of ", paste0(dispersion_types,collapse=", "), "!"))
        return(NULL)
    }

    # Check the minimum number of samples: min_samples
    if(!(is.numeric(min_samples) && is.vector(min_samples) && length(min_samples)==1 && min_samples>=1 && min_samples<=ncol(read_counts)))
    {
        warning("min_samples must be a single numerical value no less than one and no more than the number of columns of read-counts matrix!")
        return(NULL)
    }

    # Check the requirement for the minimum number of samples: min_samples_req
    min_samples_req <- tolower(min_samples_req)
    if(min_samples_req!="all" && min_samples_req!="any")
    {
        warning("min_samples_req must be either \"all\" or \"any\"!")
        return(NULL)
    }

    # Check the false discovery rate: fdr
    if(!(is.numeric(fdr) && is.vector(fdr) && length(fdr)==1 && fdr>=0 && fdr<=1))
    {
        warning("fdr must be a single numerical value between zero and one!")
        return(NULL)
    }

    # Extract required samples for DEG comparison.

    # Determine if sample groups contain the minimum number of sample replicates.
    # any: requires replicate number for each group must be greater than the threshold.
    # all: requires replicate number for at least one group must be greater than the threshold.
    min_samples_req_error <- table(group_names) < min_samples
    if(if(min_samples_req=="all") any(min_samples_req_error) else all(min_samples_req_error))
    {
        warning(paste0(paste("A minimum of", min_samples, "sample replicates are required for", (if(min_samples_req=="all") "all sample groups" else "at least one sample group")), "!"))
        return(NULL)
    }

    # Prepare the names of output files.

    # Prepare output result files.
    group_pair_file_name <- paste0(group_pair, collapse="-")
    if(!is.null(subset_name)) group_pair_file_name <- paste(group_pair_file_name, subset_name, sep=".")

    # Extract the main and extension names from output file of DEG read counts.
    if(!is.null(deg_counts_stats_file))
    {
        deg_counts_stats_file_base <- basename(deg_counts_stats_file)
        deg_counts_stats_file_dir <- dirname(deg_counts_stats_file)
        deg_counts_stats_file_main_name <- file_path_sans_ext(deg_counts_stats_file_base)
        deg_counts_stats_file_ext_name <- file_ext(deg_counts_stats_file_base)
    }
    else
    {
        deg_counts_stats_file_main_name <- NULL
        deg_counts_stats_file_ext_name <- NULL
    }
    # Generate the file name for the dataset of current DEG read counts.
    if(!is.null(deg_counts_stats_file_main_name))
    {
        # Customize file name.
        deg_counts_stats_file_name <- paste(deg_counts_stats_file_main_name, group_pair_file_name, sep=".")
        deg_counts_stats_file_name <- paste(deg_counts_stats_file_name, deg_counts_stats_file_ext_name, sep=".")
        # Add directory path.
        deg_counts_stats_file_name <- file.path(deg_counts_stats_file_dir, deg_counts_stats_file_name)
    }
    else deg_counts_stats_file_name <- NULL

    # Extract the main and extension names from output file of DEG plots.
    if(!is.null(deg_plots_file))
    {
        deg_plots_file_base <- basename(deg_plots_file)
        deg_plots_file_dir <- dirname(deg_plots_file)
        deg_plots_file_main_name <- file_path_sans_ext(deg_plots_file_base)
        deg_plots_file_ext_name <- file_ext(deg_plots_file_base)
    }
    else
    {
        deg_plots_file_main_name <- NULL
        deg_plots_file_ext_name <- NULL
    }
    # Generate the file name for the plots of current read counts and DEGs.
    if(!is.null(deg_plots_file_main_name))
    {
        # Customize file name.
        deg_plots_file_name <- paste(deg_plots_file_main_name, group_pair_file_name, sep=".")
    }
    else deg_plots_file_name <- NULL

    # Perform DEG comparison analysis.

    # Generate the output plot PDF file.
    if(!is.null(deg_plots_file_name) && nchar(deg_plots_file_name)>0 && (plot_bcv || plot_smear))
    {
        # Open a PDF file in letter size to plot
        dev_info <- openPlotDevice(dev_type=deg_plots_file_ext_name, file_name=deg_plots_file_name, dir_name=deg_plots_file_dir, width=deg_image_width, height=deg_image_height, single_file=single_deg_image_file)
        orig_par <- par(no.readonly = TRUE)
        plot_deg_flag <- TRUE
    }
    else plot_deg_flag <- FALSE

    # Construct DGEList from raw read counts and comparing group names.
    dge_profile <- createDGEList(read_counts=read_counts, group_names=group_names, ctrl_state_name=ctrl_state_name, normalize=normalize, remove_small=remove_small, norm_base=norm_base, verbose=verbose, func_dir=func_dir)
    if(is.null(batch_factors))
    {
        if(use_design_matrix) design_matrix <- model.matrix(~dge_profile$samples$group)
        else design_matrix <- NULL
    }
    else
    {
        if(length(levels(batch_factors)) > 1) design_matrix <- model.matrix(~dge_profile$samples$group+batch_factors)
        else design_matrix <- model.matrix(~dge_profile$samples$group)
    }

    group_pair_title_text <- paste0(group_pair, collapse="/")
    if(!is.null(subset_name)) group_pair_title_text <- paste(group_pair_title_text, subset_name, sep="-")

    # Estimate common, trended, and tagwise negative Binomial dispersions for entire dataset containing all combinations of interested factors.
    if(is.character(dispersion))
    {
        if(!is.null(design_matrix))
        {
            catch_result <- tryCatch(
                expr={
                    if(dispersion == "common")
                    {
                        dge_profile <- estimateGLMCommonDisp(dge_profile, design=design_matrix)
                    }
                    else if(dispersion == "trended")
                    {
                        dge_profile <- estimateGLMCommonDisp(dge_profile, design=design_matrix)
                        dge_profile <- estimateGLMTrendedDisp(dge_profile, design=design_matrix)
                    }
                    else if(dispersion == "tagwise")
                    {
                        dge_profile <- estimateGLMCommonDisp(dge_profile, design=design_matrix)
                        dge_profile <- estimateGLMTrendedDisp(dge_profile, design=design_matrix)
                        dge_profile <- estimateGLMTagwiseDisp(dge_profile, design=design_matrix)
                    }
                    else if(dispersion == "auto")
                    {
                        dge_profile <- estimateGLMCommonDisp(dge_profile, design=design_matrix)
                        dge_profile <- estimateGLMTrendedDisp(dge_profile, design=design_matrix)
                        dge_profile <- estimateGLMTagwiseDisp(dge_profile, design=design_matrix)
                    }
                },
                error=function(cond)
                {
                    # Catch errors in dispersion estimation.
                    warning("Error occurred in estimating dispersion:")
                    warning(cond)
                    # Close plot device.
                    if(plot_deg_flag)
                    {
                        par(orig_par)
                        closePlotDevice(dev_info)
                        plot_deg_flag = FALSE
                    }
                    # Return a NULL value to indicate errors.
                    return(NULL)
                }
            )
            # Abort DEG comparison when errors occur.
            if(is.null(catch_result)) return(NULL)
        }
        else
        {
            dge_profile <- estimateDisp(dge_profile)
        }
        # Only calculate and plot BCV when dispersion is available.
        if(!is.null(dge_profile$common.dispersion))
        {
            # Calculate common BCV when there are more than one sample.
            if(verbose) print(paste("BCV =", formatDecimal(sqrt(dge_profile$common.dispersion))))
            # Plot biological coefficient of variation
            if(plot_bcv)
            {
                title_text <- paste("Common BCV of", group_pair_title_text)
                title_text <- appendExprtInfo(title_text, exprt_info)
                title_text <- paste(title_text, "is", formatDecimal(sqrt(dge_profile$common.dispersion)))
                plotBCV(dge_profile, xlab="Average logCPM", ylab="Biological Coefficient of Variation", main=title_text, pch=pt_chars[1], cex.main=1.75)
            }
        }
        else
        {
            warning("Dispersion cannot be calculated due to a lack of replicates!")
            if(plot_bcv) warning("Cannot plot BCV because dispersion cannot be calculated due to a lack of replicates!")
        }
    }

    # Print comparison information to console.
    if(verbose) print(paste0(group_pair[1], " vs ", group_pair[2], ":"))

    # Perform exact test only when dispersion is available.
    dispersion_avail <- !(is.null(dge_profile$common.dispersion) && is.null(dge_profile$trended.dispersion) && is.null(dge_profile$tagwise.dispersion))
    if(dispersion_avail)
    {
        if(!is.null(design_matrix))
        {
            # Perform statistical tests based on generalized linear model for
            # gene-wise differences between two groups negative-binomially
            # distributed counts, and obtain multiple statistical measures for
            # each gene, e.g. logFC, logCPM, and p-value.
            if(!use_glm_ql)
            {
                # Use generalized linear model fitting and testing.
                dge_fit <- glmFit(dge_profile, design=design_matrix)
                dge_diffs <- glmLRT(dge_fit, coef=2)
            }
            else
            {
                # Use generalized linear model fitting with quasi-likelihood testing.
                dge_fit <- glmQLFit(dge_profile, design=design_matrix)
                dge_diffs <- glmQLFTest(dge_fit, coef=2)
            }
        }
        else
        {
            # Perform exact tests for gene-wise differences between two groups
            # of negative-binomially distributed counts, and obtain multiple
            # statistical measures for each gene, e.g. logFC, logCPM, and p-value.
            dge_diffs <- exactTest(dge_profile, pair=group_pair, dispersion=dispersion)
        }
        # Extract statistic measures and calculate adjusted p-values for each genes.
        # Do not sort top_degs, so that it can be spliced with read counts table.
        top_degs <- topTags(dge_diffs, n=nrow(dge_diffs$table), sort.by="none")
        # Print top 10 DEGs ranked by their p-value or absolute log-fold change.
        if(verbose) print((top_degs$table[sort.list(top_degs$table[,"FDR"], decreasing=FALSE),,drop=FALSE])[1:10,,drop=FALSE])

        # Classify DEG statistics as up- or down-regulation, or not significant.
        dge_pattern <- decideTestsDGE(dge_diffs, p.value=fdr)
        if(verbose) print(summary(dge_pattern))

        # Extract DEG names and flags
        deg_names <- rownames(dge_profile)[as.logical(dge_pattern)]
        degs_quant <- sum(abs(dge_pattern))

        # Plots log-Fold Change versus log-Concentration.
        if(plot_smear)
        {
            plotSmear(dge_diffs, de.tags=deg_names, pch=pt_chars[2])
            abline(h=c(-1, 1), col="blue")

            title_text <- paste(degs_quant, "DEGs")
            title_text <- paste(title_text, "in", group_pair_title_text)
            title_text <- appendExprtInfo(title_text, exprt_info)
            title_text <- paste(title_text, "at FDR", fdr)
            title(main=title_text, cex.main=1.75)
        }
    }
    else warning("DEGs cannot be calculated because dispersion is unavailable due to a lack of replicates!")

    # Merge raw and normalized read counts.
    read_counts_norm <- cpb(x=dge_profile,norm.base=norm_base)
    colnames(read_counts_norm) <- paste(colnames(read_counts_norm), "Norm", sep=".")
    dge_counts_stats <- cbind(dge_profile$counts, read_counts_norm)

    # Calculate DEGs if dispersion is available, or calculate log fold change.
    if(dispersion_avail)
    {
        # Combine read counts and DEG statistics of all genes into one matrix.
        deg_counts_stats <- cbind(dge_counts_stats, top_degs$table, Regulation=as.vector(dge_pattern))
        # If deg_only is specified, then only select DEGs for output.
        if(deg_only) deg_counts_stats <- deg_counts_stats[deg_names,,drop=FALSE]
    }
    else
    {
        warning("log-fold changes are calculated due to a lack of replicates!")
        # Log fold change may contain Inf and NaN.
        logfc <- rowMeans(dge_profile$counts[,group_names==group_pair[2],drop=FALSE]) / rowMeans(dge_profile$counts[,group_names==group_pair[1],drop=FALSE])
        # Combine read counts and log fold changes of all genes into one matrix.
        deg_counts_stats <- cbind(dge_counts_stats, logFC=logfc)
    }

    # Close plot device
    if(plot_deg_flag)
    {
        par(orig_par)
        closePlotDevice(dev_info)
        plot_deg_flag = FALSE
    }

    # Export result data files.

    # Save DEGs sorted by their range of expression level to data file
    if(!is.null(deg_counts_stats_file_name) && nchar(deg_counts_stats_file_name)>0)
    {
        # Generate R-compatible or view-compatible tab-delimited data file.
        if(r_compat) write.table(deg_counts_stats, deg_counts_stats_file_name, sep="\t", quote=FALSE)
        else write.table(cbind(Gene=rownames(deg_counts_stats), deg_counts_stats), deg_counts_stats_file_name, sep="\t", quote=FALSE, row.names=FALSE)
    }

    # Return a data.frame table of sample read counts and DEG comparison statistics.
    return(as.data.frame(deg_counts_stats))
}
