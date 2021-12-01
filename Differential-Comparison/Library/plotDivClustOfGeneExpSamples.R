# Plot divisive clusters for the dataset given state name and cell line.

plotDivClustOfGeneExpSamples <- function(divclust=NULL, group_names=NULL, group_colors=NULL, ylim=NULL, hline=NULL, hline_color="deeppink", hline_type=4, hline_width=1, xlab="Sample", ylab="Distance", title_text=NULL, title_cex=1.75, branch_line_width=2, leaf_lab_cex=0.75, ylab_cex=1.5, ytick_cex=1.5, leg_pos="topright", leg_cex=1, leg_title=NULL, leg_title_col="black", leg_title_adj=0.5, gen_empty_plot=TRUE, func_dir=NULL, ...)
{
    # divclust: the cluster object of diana type returned by calcDivClustOfGeneExpSamples.
    #           Default: NULL
    # group_names: the group name of each clustered sample in the same order as divclust$order.
    # group_colors: a named vector with its names as group names and its values as the color of each group.
    # gen_empty_plot: generate an empty plot if divclust is not specified.
    #                 Default: TRUE.

    # Load needed functions.
    require("stats")
    suppressPackageStartupMessages(require("dendextend"))

    # Load user-defined functions.
    if(is.null(func_dir)) func_dir <- getwd()
    source(file.path(func_dir, "getColors.R"), local=TRUE)

    # Check divclust argument.
    if(!is.null(divclust) && !inherits(divclust,"diana"))
    {
        # If divclust is not a diana object, do nothing.
        warning("Input clustering result must be a diana (DIvisive ANAlysis clustering) object!")
        return(divclust)
    }

    # Initialize the flag for generating a plot.
    plot_cluster <- FALSE

    # Generate a sample cluster plot or an empty plot.
    if(!is.null(divclust))
    {
        # Initialize the flag for plotting sample clusters.
        multi_sample_cluster <- TRUE

        # Check the consistency between group names and divclust.
        if(multi_sample_cluster && !is.null(group_names) && length(group_names)!=length(divclust$order))
        {
            warning("The number of group names must be equal to that of clustered samples!")
            multi_sample_cluster <- FALSE
        }

        # Check the consistency between group names and group colors.
        if(multi_sample_cluster && xor(is.null(group_names), is.null(group_colors)))
        {
            warning("group_names and group_colors must be specified or not specified at the same time!")
            multi_sample_cluster <- FALSE
        }

        # When group names and group colors are specified.
        if(multi_sample_cluster && !(is.null(group_names) && is.null(group_colors)))
        {
            # Check group colors.
            group_colors_names <- names(group_colors)
            if(!is.null(group_colors_names))
            {
                if(!any(duplicated(group_colors_names)))
                {
                    if(!setequal(group_colors_names, as.character(unique(group_names))))
                    {
                        warning("group_colors must have the same names as unique group_names!")
                        multi_sample_cluster <- FALSE
                    }
                }
                else
                {
                    warning("group_colors cannot contain duplicate names!")
                    multi_sample_cluster <- FALSE
                }
            }
            else
            {
                warning("group_colors must be a vector of named color values!")
                multi_sample_cluster <- FALSE
            }
        }

        # Generate a hierarchical clustering plot.
        if(multi_sample_cluster)
        {
            # Set the sizes of branches and labels.
            dendro <- as.dendrogram(divclust)
            dendro <- set(dendro, "branches_lwd", branch_line_width)
            dendro <- set(dendro, "labels_cex", leaf_lab_cex)
            # Set the colors of cluster branches and leaf labels according to sample groups.
            if(!is.null(group_names))
            {
                uniq_groups <- as.character(sort(unique(group_names)))
                if(is.null(group_colors))
                {
                    group_colors <- getColors(n=length(uniq_groups))
                    names(group_colors) <- uniq_groups
                }
                dendro_colors <- group_colors[as.character(group_names)]
                dendro <- color_branches(dendro, col=dendro_colors)
                dendro <- color_labels(dendro, col=dendro_colors)
            }

            # Plot the hierarchical tree of sample clusters.
            plot(dendro, ylim=ylim, main=title_text, cex.main=title_cex, ylab=ylab, cex.lab=ylab_cex, cex.axis=ytick_cex, ...)
            if(!is.null(group_names)) legend(leg_pos, legend=uniq_groups, text.col=group_colors[uniq_groups], title=leg_title, title.col=leg_title_col, title.adj=leg_title_adj, bty='n', cex=leg_cex)

            # Set the plot flag.
            plot_cluster <- TRUE
        }
    }
    else if(gen_empty_plot)
    {
        # Generate an empty plot.
        plot(as.dendrogram(hclust(dist(matrix(1,2,2)))), main=title_text, cex.main=title_cex, ylab=ylab, ylim=ylim, leaflab="none", edgePar=list(lty=0), ...)

        # Set the plot flag.
        plot_cluster <- TRUE
    }


    # Add a horizontal line.
    if(plot_cluster && !is.null(hline)) abline(h=hline, col=hline_color, lty=hline_type, lwd=hline_width)

    # Return the results.
    invisible(divclust)
}
