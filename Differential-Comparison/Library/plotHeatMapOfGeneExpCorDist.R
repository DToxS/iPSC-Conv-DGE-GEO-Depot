# Plot heat map of gene expression distance using count per million (CPM) reads.
# The CPM trnsformation of raw read counts normalizes gene expression level
# over entire sample, so that the difference of expression level due to varied
# reading depth of each sample is eliminated and therefore different samples
# can be compared directly in terms of their gene expression profile.
# NOTE: CPM normalization doesn not change the relative expression level of
# a gene compared with that of other genes in the same sample, and therefore
# the difference of expression levels between different genes is maintained.

plotHeatMapOfGeneExpCorDist <- function(gene_exp, gene_list=NULL, group_names=NULL, use_group_names=FALSE, sort_group_names=FALSE, cross_group_only=FALSE, dendrogram=FALSE, map_type="distance", dist_method="correlation", cor_method="pearson", normalize=FALSE, norm_factors=FALSE, norm_genes=FALSE, log=FALSE, remove_small=FALSE, min_value=NULL, max_value=NULL, lower_limit=NULL, upper_limit=NULL, color_map_type=NULL, key_size=NULL, key_title=NULL, key_xlabel=NULL, key_ylabel=NULL, color_pal=NULL, n_color=500, title_text=NULL, label_size=NULL, margin_bottom=5, margin_right=margin_bottom, func_dir=NULL, ...)
{
    # gene_exp: either a numerical matrix of raw read counts or a DGEList object of edgeR.
    # gene_list <- NULL # A list of selected genes to plot
    # group_names <- c("Wildtype", "Wildtype", "Knockout", "Knockout", "Mixed", "Mixed")
    # use_group_names <- FALSE
    # sort_group_names <- FALSE
    # dendrogram <- fALSE
    # map_type <- "distance"
    # dist_method <- "euclidean"
    # norm_factors <- FALSE
    # normalize <- FALSE
    # norm_genes <= FALSE
    # log <- FALSE
    # remove_small <- FALSE
    # min_value <- NULL
    # max_value <- NULL
    # lower_limit <- NULL
    # upper_limit <- NULL
    # color_map_type <- NULL
    # key_size <- NULL
    # key_title <- NULL
    # key_xlabel <- NULL
    # key_ylabel <- NULL
    # color_pal <- NULL
    # n_color <- 500
    # title_text <- NULL
    # label_size <- NULL
    # margin_bottom <- 5
    # margin_right <- margin_bottom
    # func_dir <- "/Users/granville/Documents/works/academia/MSSM/LINCS/Programs/"

    # Load required library
    require(stats)
    require(gplots)
    require(edgeR)

    # Load user-defined functions.
    if(is.null(func_dir)) func_dir <- getwd()
    source(file.path(func_dir, "createDGEList.R"), local=TRUE)
    source(file.path(func_dir, "cpb.R"), local=TRUE)
    source(file.path(func_dir, "cpb.default.R"), local=TRUE)
    source(file.path(func_dir, "cpb.DGEList.R"), local=TRUE)
    source(file.path(func_dir, "generateColorGradient.R"), local=TRUE)

    # Check map_type argument.
    if(map_type!="distance" && map_type!="correlation")
    {
        warning("map_type must be either distance or correlation!")
        return(NULL)
    }

    # Check distance and correlation methods.
    if(dist_method!="correlation" && dist_method!="euclidean")
    {
        warning("dist_method must be either correlation or euclidean!")
        return(NULL)
    }
    if(cor_method!="pearson" && cor_method!="spearman")
    {
        warning("cor_method must be either pearson or spearman!")
        return(NULL)
    }

    # Check group names.
    if(cross_group_only && is.null(group_names))
    {
        warning("group_names cannot be NULL when cross_group_only is TRUE!")
        return(NULL)
    }

    # Check the availability of min and max values.
    if(!((is.null(min_value)||is.null(max_value)) == (is.null(min_value)&&is.null(max_value))))
    {
        warning("min_value and max_value must be either both NULL or both not NULL!")
        return(NULL)
    }

    # Check the availability of lower and upper limits.
    if(!((is.null(lower_limit)||is.null(upper_limit)) == (is.null(lower_limit)&&is.null(upper_limit))))
    {
        warning("lower_limit and upper_limit must be either both NULL or both not NULL!")
        return(NULL)
    }

    # Check color_map_type argument
    if(!is.null(color_map_type) && color_map_type!="histogram" && color_map_type!="bar")
    {
        warning("color_map_type must be one of histogram, bar, or NULL!")
        return(NULL)
    }

    # Determine the data type of input gene_exp.
    dge_profile <- NULL
    if(inherits(gene_exp, "DGEList"))
    {
        dge_profile <- gene_exp
        if(is.null(group_names)) group_names <- as.character(dge_profile$samples$group)
        else dge_profile$samples$group <- as.factor(group_names)
        # Generate a normalized read counts matrix.
        if(normalize) read_counts_cpm <- cpb(dge_profile, normalized.lib.sizes=norm_factors, log=log, shared.genes=norm_genes)
        else read_counts_cpm <- dge_profile$counts
    }
    else if(is.matrix(gene_exp) && is.numeric(gene_exp))
    {
        read_counts <- gene_exp
        if(ncol(read_counts) > 1)
        {
            # Create DGEList.
            if(is.null(group_names)) group_names <- rep(1, ncol(read_counts))
            # Generate a normalized read counts matrix.
            if(normalize) read_counts_cpm <- cpb(read_counts, log=log, shared.genes=norm_genes)
            else read_counts_cpm <- read_counts
        }
        else
        {
            # Non-fatal error.
            warning("Read counts must contain more than one observation!")
            return(NULL)
        }
    }
    else
    {
        warning("Input gene_exp must be either a numeric matrix or a DGEList object!")
        return(NULL)
    }

    # Extract only list genes.
    if(!is.null(gene_list))
    {
        if(is.vector(gene_list))
        {
            # Take the intersect of gene_list and read_counts_cpm due to the removal of small counts.
            gene_list_common <- intersect(gene_list, rownames(read_counts_cpm))
            read_counts_cpm <- read_counts_cpm[gene_list_common,]
        }
        else stop("Gene list must be a vector!")
    }

    # Use sample conditions to re-order the read counts matrix.
    if(use_group_names)
    {
        if(sort_group_names)
        {
            # Re-order sample columns according to their conditions.
            ss_names <- rbind(colnames(read_counts_cpm), group_names)
            ss_names <- ss_names[,sort.list(ss_names[2,], decreasing=FALSE)]
            read_counts_cpm <- read_counts_cpm[,ss_names[1,]]
            # Set column names to condition names for heat map plot.
            colnames(read_counts_cpm) <- ss_names[2,]
        }
        else
        {
            # Set column names to condition names for heat map plot.
            colnames(read_counts_cpm) <- group_names
        }
    }

    # Create a color pallette
    if(is.null(color_pal))
    {
        if(map_type == "distance") color_pal <- c("black", "red", "yellow", "white")
        else if(map_type == "correlation") color_pal <- c("blue", "white", "red")
    }
    stopifnot(!is.null(color_pal))

    # Generate a display color palette from given total and display value ranges.
    if(!is.null(lower_limit) || !is.null(upper_limit))
    {
        if(!is.null(min_value) || !is.null(max_value)) color_map <- generateColorGradient(pal=color_pal, n_color=n_color, val_min=min_value, val_max=max_value, val_min_disp=lower_limit, val_max_disp=upper_limit)
        else color_map <- generateColorGradient(pal=color_pal, n_color=n_color, val_min=lower_limit, val_max=upper_limit)
    }
    else
    {
        if(!is.null(min_value) || !is.null(max_value)) color_map <- generateColorGradient(pal=color_pal, n_color=n_color, val_min=min_value, val_max=max_value)
        else color_map <- generateColorGradient(pal=color_pal, n_color=n_color)
    }

    # Calculate a matrix of distance or correlation between multiple samples.
    cor_dists_mat <- NULL
    if(map_type == "distance")
    {
        if(dist_method != "correlation") cor_dists <- dist(t(read_counts_cpm), method=dist_method)
        else cor_dists <- as.dist((1 - cor(read_counts_cpm,method=cor_method)) / 2)
        cor_dists_mat <- as.matrix(cor_dists)
    }
    else if(map_type == "correlation") cor_dists_mat <- cor(read_counts_cpm, method=cor_method)
    stopifnot(!is.null(cor_dists_mat))
    rownames(cor_dists_mat) <- colnames(cor_dists_mat) <- colnames(read_counts_cpm)

    # Extract cross-group distance or correlation if required.
    # Note: only the first 2 groups are used.
    cor_dists_mat_sub <- cor_dists_mat
    if(cross_group_only)
    {
        group_names_unq <- unique(group_names)
        if(length(group_names_unq) > 1)
        {
            # Get the first two group names.
            group_name_1 <- group_names_unq[1]
            group_name_2 <- group_names_unq[2]
            # Retrieve group 1 from rows and group 2 from columns.
            cor_dists_mat_sub <- cor_dists_mat_sub[group_names==group_name_1,group_names==group_name_2]
        }
    }

    # Customize color map according to lower and upper limits.
    breaks <- NULL
    if(!is.null(lower_limit) | !is.null(upper_limit))
    {
        cor_dists_mat_sub[cor_dists_mat_sub < lower_limit] <- lower_limit
        cor_dists_mat_sub[cor_dists_mat_sub > upper_limit] <- upper_limit
        breaks <- seq(lower_limit, upper_limit, length.out=(n_color+1))
    }

    # Set dendrogram parameters.
    rowv <- dendrogram
    symm <- TRUE
    dendrogram <- (if(dendrogram) "both" else "none")

    # Set color key/legend parameters.

    # Determine whether or not to plot color key, and, if not,
    # determine the padding space based on the presence of key
    # title.
    if(!is.null(color_map_type))
    {
        key <- TRUE

        # Set the type of color key.
        if(color_map_type == "histogram") density.info <- "histogram"
        else if(color_map_type == "bar") density.info <- "none"
        else density.info <- "none"

        # Set the size of color key.
        if(is.null(key_size)) key_size <- 1.25

        # Set the title of color key.
        if(is.null(key_title)) key_title <- "Expression Level"
        # Set the axis labels of color key.
        if(is.null(key_xlabel))
        {
            if(map_type == "distance")
            {
                if(dist_method != "correlation")
                {
                    if(log) key_xlabel <- "logCPM"
                    else key_xlabel <- "CPM"
                }
                else key_xlabel <- "-R*"
            }
            else if(map_type == "correlation") key_xlabel <- "R"
            else key_xlabel <- "Level"
        }
        if(is.null(key_ylabel) && color_map_type=="histogram") key_ylabel <- "Count"
    }
    else
    {
        key <- FALSE
        density.info <- "none"
        if(is.null(key_size))
        {
            if(!is.null(title_text)) key_size <- 0.5
            else key_size <- 0.1
        }
    }

    # Plot the heat map
    if(!is.null(label_size)) heatmap.2(cor_dists_mat_sub, Rowv=rowv, symm=TRUE, dendrogram=dendrogram, scale="none", trace="none", breaks=breaks, col=color_map, main=title_text, key=key, keysize=key_size, density.info=density.info, key.title=key_title, key.xlab=key_xlabel, key.ylab=key_ylabel, cexRow=label_size, cexCol=label_size, margins=c(margin_bottom,margin_right), ...)
    else heatmap.2(cor_dists_mat_sub, Rowv=rowv, symm=TRUE, dendrogram=dendrogram, scale="none", trace="none", breaks=breaks, col=color_map, main=title_text, key=key, keysize=key_size, density.info=density.info, key.title=key_title, key.xlab=key_xlabel, key.ylab=key_ylabel, margins=c(margin_bottom,margin_right), ...)

    # Return CPM read counts implicitly.
    invisible(read_counts_cpm)
}
