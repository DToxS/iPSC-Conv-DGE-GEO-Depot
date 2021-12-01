# Get a set of colors from selected color palettes comprised by distinctive
# color groups.

getColors <- function(n=NULL, pals=NULL, n_pal_colors=6, beg=1, display=FALSE)
{
    # Input arguments
    # n: the number of colors to generate (can be any positive number).
    # pals: the names of predefined color palettes to use: red, magenta, brown, grey, green, blue, purple.
    # n_pal_colors: the number of predefined colors to select from each palette: must be within 1 to 10-beg.
    # beg: the beginning position of color selection from each palette: must be within 1 to 9.
    # display: the flag for displaying selected colors.

    # Load required liblraries.
    require("RColorBrewer")

    # Pre-defined color palettes.
    pred_pals <- c(red="Reds", magenta="RdPu", brown="YlOrBr", grey="Greys", green="Greens", blue="Blues", purple="BuPu")
    pred_n_pal_colors <- 9

    # Check input arguments.
    stopifnot(is.null(n) || (is.numeric(n) && length(n)==1 && n>=1))
    stopifnot(is.null(pals) || all(pals %in% names(pred_pals)))
    stopifnot(is.numeric(beg) && length(beg)==1 && beg%%1==0 && beg>=1 && beg<=pred_n_pal_colors)
    stopifnot(is.numeric(n_pal_colors) && length(n_pal_colors)==1 && n_pal_colors%%1==0 && n_pal_colors>=1 && n_pal_colors<=(pred_n_pal_colors-beg+1))
    stopifnot(is.logical(display))

    # Hand pick a set of color palettes.
    pal_color_end <- pred_n_pal_colors - beg + 1
    pal_color_beg <- pal_color_end - n_pal_colors + 1
    if(!is.null(pals)) sel_pals <- pred_pals[pals]
    else sel_pals <- pred_pals

    pal_colors <- c()
    for(pal_name in sel_pals) pal_colors <- c(pal_colors, brewer.pal(pred_n_pal_colors,pal_name)[pal_color_beg:pal_color_end])

    # Get the given number of colors from the hand-picked colors.
    # Note: if the number of colors needed is more than that of avaialble
    # hand-picked colors, the available colors will be reused by cycling.
    if(!is.null(n))
    {
        n_pal_colors <- length(pal_colors)
        color_pos <- c(1:n) %% n_pal_colors
        color_pos[color_pos==0] <- n_pal_colors
        sel_colors <- pal_colors[color_pos]
    }
    else sel_colors <- pal_colors

    # Display the color bars.
    if(display) barplot(rep(5,length(sel_colors)), col=sel_colors, names.arg=sel_colors, las=2)

    # Return the color set.
    return(sel_colors)
}
