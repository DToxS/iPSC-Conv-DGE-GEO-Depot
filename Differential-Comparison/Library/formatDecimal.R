# Format the digits after decimal point.

formatDecimal <- function(x, k=2, scientific=FALSE, drop0trailing=FALSE)
{
    # x is a numerical number.
    # k is the number of digits after decimal point.

    #if(scientific) r <- format(x, digits=k+1, scientific=TRUE, drop0trailing=drop0trailing, trim=TRUE)
    #else r <- format(round(x, k), nsmall=k, scientific=FALSE, drop0trailing=drop0trailing, trim=TRUE)
    if(scientific) val_format <- "e" else val_format <- "f"
    r <- formatC(round(x, k), digits=k, format=val_format, drop0trailing=drop0trailing)

    return(r)
}
