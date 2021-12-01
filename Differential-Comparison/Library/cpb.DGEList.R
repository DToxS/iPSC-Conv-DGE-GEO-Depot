cpb.DGEList <- function (x, normalized.lib.sizes=TRUE, log=FALSE, prior.count=0.25, norm.base=1e+6, shared.genes=FALSE, ...)
{
    # Load required library
    require(methods)

    lib.size <- x$samples$lib.size
    if(normalized.lib.sizes) lib.size <- lib.size * x$samples$norm.factors
    cpb.default(x$counts, lib.size=lib.size, log=log, prior.count=prior.count, norm.base=norm.base, shared.genes=shared.genes)
}
