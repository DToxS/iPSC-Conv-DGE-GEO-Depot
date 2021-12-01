cpb.default <- function(x, lib.size=NULL, log=FALSE, prior.count=0.25, norm.base=1e+6, shared.genes=FALSE, ...)
{
    x <- as.matrix(x)
    if(is.null(lib.size))
    {
        if(shared.genes)
        {
            x.nz <- x[rowSums(x==0)==0,]
            lib.size <- colSums(x.nz)
        }
        else lib.size <- colSums(x)
    }
    if(log)
    {
        prior.count.scaled <- lib.size/mean(lib.size) * prior.count
        lib.size <- lib.size + 2 * prior.count.scaled
    }
    lib.size <- lib.size / norm.base
    if(log) log2(t((t(x) + prior.count.scaled)/lib.size))
    else t(t(x)/lib.size)
}
