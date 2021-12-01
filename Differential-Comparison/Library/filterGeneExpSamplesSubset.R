# Filter outlier samples by removing outlier samples.
# Note:

filterGeneExpSamplesSubset <- function(subset_name, exprt_design, read_counts, cutoff, hline, dist_cutoff_outlier=0.01, dist_cutoff_group=0.015, min_samples=3, filter_outlier=TRUE, keep_under_samples=FALSE, plot_orig_clust=FALSE, plot_filter_clust=FALSE, plot_pass_clust=FALSE, plot_empty_clust=FALSE, color_sample_groups=TRUE, group_colors=NULL, color_field_pos=3, color_field_type="numeric", hline_color="deeppink", hline_type=4, hline_width=1, clust_title="Clustered Samples", title_cex=1.75, branch_line_width=2, leaf_lab_cex=0.75, ylab_cex=1.5, leg_title="Culture", leg_pos="topright", leg_cex=1, leg_title_col="black", leg_title_adj=1, verbose=FALSE, func_dir=NULL)
{
    # plot_orig_clust: plot the sample clusters for input samples.
    #                  Default: FALSE
    # plot_filter_clust: plot the sample clusters for filtered samples.
    #                    Default: FALSE
    # plot_pass_clust: plot the sample clusters for non-filtered samples.
    #                        Default: FALSE

    # Load required library
    require("matrixStats")

    # Load user-defined functions.
    if(is.null(func_dir)) func_dir <- getwd()
    source(file.path(func_dir, "getColors.R"), local=TRUE)
    source(file.path(func_dir, "clusterGeneExpSamples.R"), local=TRUE)
    source(file.path(func_dir, "selectGeneExpSamples.R"), local=TRUE)
    source(file.path(func_dir, "plotDivClustOfGeneExpSamples.R"), local=TRUE)

    # Check read_counts argument.
    if(!(is.matrix(read_counts) && is.numeric(read_counts)))
    {
        warning("Sample read counts must be a numeric matrix!")
        return(NULL)
    }

    # Check the consistency between read_counts and exprt_design.
    if(ncol(read_counts) != nrow(exprt_design))
    {
        warning("The number of read_counts columns must be same as the number of exprt_design rows!")
        return(NULL)
    }

    # Set the parameters for color palette.
    color_pals <- c("red", "blue", "green", "brown", "purple", "magenta")
    n_pal_colors <- 4

    # Set group names.
    if(plot_orig_clust && color_sample_groups && is.matrix(read_counts))
    {
        group_names <- sapply(strsplit(colnames(read_counts),split="\\."), function(x){x[color_field_pos]})
        if(color_field_type == "numeric") group_names <- as.numeric(group_names)
    }
    else group_names <- NULL

    # Set group colors according to group names.
    if(!is.null(group_names) && is.null(group_colors))
    {
        unique_group_names <- sort(unique(group_names))
        group_colors <- getColors(n=length(unique_group_names), pals=color_pals, n_pal_colors=n_pal_colors)
        names(group_colors) <- as.character(unique_group_names)
    }

    # Calculate and plot divisive clusters for the samples in current cell line and drug-treated condition.
    original_divclust_results <- clusterGeneExpSamples(read_counts=read_counts, group_names=group_names, group_colors=group_colors, plot_clust=plot_orig_clust, hline=hline, hline_color=hline_color, hline_type=hline_type, hline_width=hline_width, title_text=paste(ncol(read_counts),clust_title), title_cex=title_cex, branch_line_width=branch_line_width, leaf_lab_cex=leaf_lab_cex, ylab_cex=ylab_cex, leg_pos=leg_pos, leg_cex=leg_cex, leg_title=leg_title, leg_title_col=leg_title_col, leg_title_adj=leg_title_adj, func_dir=func_dir)

    # Generate an empty cluster plot if clusterGeneExpSamples fails but
    # a cluster plot is required for original input samples.
    if(is.null(original_divclust_results) && ncol(read_counts)>0 && plot_orig_clust && plot_empty_clust)
    {
        # Plot an empty cluster.
        if(is.matrix(read_counts) && ncol(read_counts)>min_samples) ylim <- c(0, dist_cutoff_outlier)
        else ylim <- c(0, dist_cutoff_group)
        hline <- ylim[2]
        plotDivClustOfGeneExpSamples(ylim=ylim, hline=hline, hline_color=hline_color, hline_type=hline_type, hline_width=hline_width, title_text=paste(ncol(read_counts),clust_title), title_cex=title_cex, branch_line_width=branch_line_width, leaf_lab_cex=leaf_lab_cex, ylab_cex=ylab_cex, leg_pos=leg_pos, leg_cex=leg_cex, leg_title_col=leg_title_col, leg_title_adj=leg_title_adj, func_dir=func_dir)
    }

    # Filter out outlier samples according to given correlation threshold.
    if(filter_outlier)
    {
        # Set title text.
        clust_title_filter <- paste(clust_title, "(Filtered)")

        # Filter outlier sample replicates according to given cutoffs.
        sample_names_sel <- selectGeneExpSamples(read_counts, cutoff_outlier=cutoff[1], cutoff_group=cutoff[2], min_samples=min_samples, keep_under_samples=keep_under_samples, verbose=verbose, func_dir=func_dir)
        exprt_design_sel <- exprt_design[exprt_design$ID%in%sample_names_sel,,drop=FALSE]

        # Plot divisive clusters of filtered samples.
        if(nrow(exprt_design_sel) > 0)
        {
            # Extract the read counts of selected samples.
            read_counts_sel <- read_counts[,sample_names_sel,drop=FALSE]
            read_counts_sel <- read_counts_sel[!rowAnys(is.na(read_counts_sel)),,drop=FALSE]

            # Set group names and group colors to highlight clustered samples.
            if(plot_filter_clust && color_sample_groups && is.matrix(read_counts_sel))
            {
                # Set group names according to selected samples.
                group_names <- sapply(strsplit(colnames(read_counts_sel),split="\\."), function(x){x[color_field_pos]})
                if(color_field_type == "numeric") group_names <- as.numeric(group_names)
                # Set group colors according to group names.
                unique_group_names <- sort(unique(group_names))
                group_colors <- getColors(n=length(unique_group_names), pals=color_pals, n_pal_colors=n_pal_colors)
                names(group_colors) <- as.character(unique_group_names)
            }
            else
            {
                group_names <- NULL
                group_colors <- NULL
            }
            # Calculate and plot divisive clusters for filtered samples after outlier removal.
            filtered_divclust_results <- clusterGeneExpSamples(read_counts=read_counts_sel, group_names=group_names, group_colors=group_colors, plot_clust=plot_filter_clust, ylim=original_divclust_results$ylim, hline=original_divclust_results$hline, hline_color=hline_color, hline_type=hline_type, hline_width=hline_width, title_text=paste(ncol(read_counts_sel),clust_title_filter), title_cex=title_cex, branch_line_width=branch_line_width, leaf_lab_cex=leaf_lab_cex, ylab_cex=ylab_cex, leg_pos=leg_pos, leg_cex=leg_cex, leg_title=leg_title, leg_title_col=leg_title_col, leg_title_adj=leg_title_adj, func_dir=func_dir)

            # Generate an empty cluster plot if clusterGeneExpSamples fails but
            # a cluster plot is required for original input samples.
            if(is.null(filtered_divclust_results) && ncol(read_counts_sel)>0 && plot_filter_clust && (!is.null(original_divclust_results) || (is.null(original_divclust_results)&&plot_empty_clust)))
            {
                # Plot an empty cluster.
                if(is.matrix(read_counts_sel) && ncol(read_counts_sel)>min_samples) ylim <- c(0, dist_cutoff_outlier)
                else ylim <- c(0, dist_cutoff_group)
                hline <- ylim[2]
                plotDivClustOfGeneExpSamples(ylim=ylim, hline=hline, hline_color=hline_color, hline_type=hline_type, hline_width=hline_width, title_text=paste(ncol(read_counts_sel),clust_title_filter), title_cex=title_cex, branch_line_width=branch_line_width, leaf_lab_cex=leaf_lab_cex, ylab_cex=ylab_cex, leg_pos=leg_pos, leg_cex=leg_cex, leg_title_col=leg_title_col, leg_title_adj=leg_title_adj, func_dir=func_dir)
            }
        }
        else
        {
            # Generate an empty cluster plot if all original samples are filtered out.
            if(plot_filter_clust && (!is.null(original_divclust_results) || (is.null(original_divclust_results)&&plot_empty_clust)))
            {
                # Plot a filtered empty cluster because the number of samples is less than
                # min_samples.
                if(!is.null(original_divclust_results))
                {
                    ylim <- c(0, original_divclust_results$ylim)
                    hline <- original_divclust_results$hline
                }
                else
                {
                    if(is.matrix(read_counts_sel) && ncol(read_counts_sel)>min_samples) ylim <- c(0, dist_cutoff_outlier)
                    else ylim <- c(0, dist_cutoff_group)
                    hline <- ylim[2]
                }
                plotDivClustOfGeneExpSamples(ylim=ylim, hline=hline, hline_color=hline_color, hline_type=hline_type, hline_width=hline_width, title_text=paste(nrow(exprt_design_sel),clust_title_filter), title_cex=title_cex, branch_line_width=branch_line_width, leaf_lab_cex=leaf_lab_cex, ylab_cex=ylab_cex, leg_pos=leg_pos, leg_cex=leg_cex, leg_title_col=leg_title_col, leg_title_adj=leg_title_adj, func_dir=func_dir)
            }
        }
    }
    else
    {
        exprt_design_sel <- exprt_design
        if(plot_pass_clust)
        {
            # Set title text.
            clust_title_pass <- paste(clust_title, "(Passed)")
            # All replicate samples pass through.
            if(!is.null(original_divclust_results))
            {
                if(!is.null(original_divclust_results) && is.list(original_divclust_results) && !is.null(original_divclust_results$clust))
                {
                    # Plot the original cluster again.
                    if(!is.null(original_divclust_results$ylim)) ylim <- c(0, original_divclust_results$ylim)
                    else ylim <- c(0, dist_cutoff_outlier)
                    if(!is.null(original_divclust_results$hline)) hline <- original_divclust_results$hline
                    else hline <- ylim[2]
                    plotDivClustOfGeneExpSamples(divclust=original_divclust_results$clust, ylim=ylim, hline=hline, hline_color=hline_color, hline_type=hline_type, hline_width=hline_width, title_text=paste(ncol(read_counts),clust_title_pass), title_cex=title_cex, branch_line_width=branch_line_width, leaf_lab_cex=leaf_lab_cex, ylab_cex=ylab_cex, leg_pos=leg_pos, leg_cex=leg_cex, leg_title_col=leg_title_col, leg_title_adj=leg_title_adj, func_dir=func_dir)
                }
            }
            else if(plot_empty_clust)
            {
                # Plot an empty cluster.
                ylim <- c(0, dist_cutoff_outlier)
                hline <- ylim[2]
                plotDivClustOfGeneExpSamples(ylim=ylim, hline=hline, hline_color=hline_color, hline_type=hline_type, hline_width=hline_width, title_text=paste(ncol(read_counts),clust_title_pass), title_cex=title_cex, branch_line_width=branch_line_width, leaf_lab_cex=leaf_lab_cex, ylab_cex=ylab_cex, leg_pos=leg_pos, leg_cex=leg_cex, leg_title_col=leg_title_col, leg_title_adj=leg_title_adj, func_dir=func_dir)
            }
        }
    }

    # Return filtered experiment design table.
    return(exprt_design_sel)
}
