# Calculate and plot divisive clusters of gene expression read counts.
# Note: this function returns NULL for invalid input arguments.

clusterGeneExpSamples <- function(read_counts, group_names=NULL, group_colors=NULL, plot_clust=FALSE, ylim=NULL, hline=NULL, hline_color="deeppink", hline_type=4, hline_width=1, title_text=NULL, title_cex=1.75, branch_line_width=2, leaf_lab_cex=0.75, ylab_cex=1.5, ytick_cex=1.5, leg_pos="topright", leg_cex=1, leg_title=NULL, leg_title_col="black", leg_title_adj=0.5, func_dir=NULL, ...)
{
    # Input arguments
    # read_counts: the read counts of multiple samples with gene rows and sample columns.
    # group_names: the group name for each sample columns of read counts.
    # group_colors: a named vector with its names as group names and its values as the color of each group.
    # ylim: the upper limit of Y axis (a single value).
    # hline: the Y value of the threshold hold to draw.

    # Load required library
    require(stats)
    require(matrixStats)

    # Load user-defined functions.
    if(is.null(func_dir)) func_dir <- getwd()
    source(file.path(func_dir, "getCutoff.R"), local=TRUE)
    source(file.path(func_dir, "calcInvExp10ofDecimal.R"), local=TRUE)
    source(file.path(func_dir, "calcDivClustOfGeneExpSamples.R"), local=TRUE)
    source(file.path(func_dir, "plotDivClustOfGeneExpSamples.R"), local=TRUE)

    # Check read_counts argument.
    if(!(is.matrix(read_counts) && ncol(read_counts)>1 && is.numeric(read_counts)))
    {
        warning("Sample read counts must be a numeric matrix with two columns at least for clustering analysis!")
        return(NULL)
    }

    # Remove the rows with NA.
    read_counts <- read_counts[!rowAnyNAs(read_counts),,drop=FALSE]

    # Initialize the result set.
    results <- list()

    # Calculate divisive clusters for the samples in current cell line and drug-treated condition.
    divclust <- calcDivClustOfGeneExpSamples(read_counts=read_counts, metric="correlation")

    # Plot divisive clusters of orginal samples.
    if(plot_clust)
    {
        # Calculate the upper limit of Y axis for the cluster tree plot.
        if(is.null(ylim))
        {
            dist_max <- (1 - min(cor(read_counts))) / 2
            inv_exp10 <- calcInvExp10ofDecimal(dist_max)
            dist_max_mag <- dist_max * 10^inv_exp10
            dist_max_mag_round <- round(dist_max_mag)
            dist_max_mag_floor <- floor(dist_max_mag)
            dist_max_mag_ceiling <- ceiling(dist_max_mag)
            if(dist_max_mag_round == dist_max_mag_floor) ylim <- (dist_max_mag_floor + 0.5) / 10^inv_exp10
            else ylim <- dist_max_mag_ceiling / 10^inv_exp10
        }
        # Adjust ylim if hline is above it.
        if(!is.null(hline) && is.infinite(hline)) hline <- NULL
        if(is.null(ylim))
        {
            if(!is.null(hline)) ylim <- ceiling(hline * 10^inv_exp10) / 10^inv_exp10
        }
        else
        {
            if(!is.null(hline) && hline > ylim) ylim <- hline
        }
        if(!is.null(ylim)) ylim <- c(0, ylim)
        # Reorder the group names of the samples according to their order in the
        # clustering results.
        if(!is.null(group_names)) group_names <- group_names[divclust$order]
        # Plot divisive clusters of filtered samples.
        plotDivClustOfGeneExpSamples(divclust=divclust, group_names=group_names, group_colors=group_colors, ylim=ylim, hline=hline, hline_color=hline_color, hline_type=hline_type, hline_width=hline_width, title_text=title_text, title_cex=title_cex, branch_line_width=branch_line_width, leaf_lab_cex=leaf_lab_cex, ylab_cex=ylab_cex, ytick_cex=ytick_cex, leg_pos=leg_pos, leg_cex=leg_cex, leg_title=leg_title, leg_title_adj=leg_title_adj, func_dir=func_dir, ...)
        if(!is.null(ylim)) ylim <- ylim[2]
    }

    # Assemble the result set.
    if(!is.null(divclust)) results[["clust"]] <- divclust
    if(!is.null(ylim)) results[["ylim"]] <- ylim
    if(!is.null(hline)) results[["hline"]] <- hline
    if(length(results) == 0) results <- NULL

    # Return cluster results.
    return(results)
}
