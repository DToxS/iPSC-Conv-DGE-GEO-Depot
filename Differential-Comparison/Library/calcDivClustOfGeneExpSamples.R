# Calculate divisive hierarchical clusters of given gene expression samples.
# Note: this function returns NULL for invalid input arguments.

calcDivClustOfGeneExpSamples <- function(read_counts, metric="correlation")
{
    # read_counts: a matrix of gene expression samples.
    # metric: "correlation", "euclidean", or "manhattan".

    # Load needed functions.
    require(stats)
    require(cluster)

    # Check input read counts matrix.
    if(!(is.matrix(read_counts) && ncol(read_counts)>1 && is.numeric(read_counts)))
    {
        warning("Input read counts must be a numeric matrix with two sample columns at least!")
        return(NULL)
    }

    # Check input distance metric.
    if(!(metric=="correlation" || metric=="euclidean" || metric=="manhattan"))
    {
        warning("Input metric must be one of correlatioin, euclidean or manhattan!")
        return(NULL)
    }

    # Decide which method to use for calculation.
    if(metric == "correlation")
    {
        # Calculate the distance matrix between sample columns based on their correlations.
        dist_mat <- as.dist((1-cor(read_counts))/2)
        # Performe a divisive hierarchical clustering analysis on input distance matrix
        # and return a diana object.
        divclust <- diana(dist_mat)
    }
    else divclust <- diana(t(read_counts), metric=metric)

    # Return the results.
    return(divclust)
}
