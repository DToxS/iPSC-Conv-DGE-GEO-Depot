# Generate a gradient from a set of given colors based on
# a total range and a display range.

generateColorGradient <- function(n_color=100, pal=NULL, val_min=NULL, val_max=NULL, val_min_disp=NULL, val_max_disp=NULL, test=FALSE)
{
    # Load required library
    require(graphics)

    # Check the validity of specified color palette.
    if(!is.null(pal))
    {
        total_colors <- colors()
        pal_validity <- all(pal %in% total_colors)
        stopifnot(pal_validity)
    }

    # Check the availability of min and max values.
    if(!((is.null(val_min)||is.null(val_max)) == (is.null(val_min)&&is.null(val_max))))
    {
        warning("val_min and val_max must be either both NULL or both not NULL!")
        return(NULL)
    }
    if(!((is.null(val_min_disp)||is.null(val_max_disp)) == (is.null(val_min_disp)&&is.null(val_max_disp))))
    {
        warning("val_min_disp and val_max_disp must be either both NULL or both not NULL!")
        return(NULL)
    }
    if(!is.null(val_min) || !is.null(val_max)) stopifnot(val_min <= val_max)
    if(!is.null(val_min_disp) || !is.null(val_max_disp)) stopifnot(val_min_disp <= val_max_disp)
    if((!is.null(val_min) || !is.null(val_max)) && (!is.null(val_min_disp) || !is.null(val_max_disp)))
    {
        stopifnot(val_min <= val_min_disp)
        stopifnot(val_max_disp <= val_max)
    }
    if(!is.null(val_min) && is.null(val_min_disp))
    {
        val_min_disp <- val_min
        val_max_disp <- val_max
    }
    if(is.null(val_min) && !is.null(val_min_disp))
    {
        val_min <- val_min_disp
        val_max <- val_max_disp
    }

    # Create the palette.
    if(!is.null(pal)) color_pal <- colorRampPalette(pal)
    else color_pal <- rainbow

    # Calculate the color map to display.
    if(!is.null(val_min) && !is.null(val_max) && !is.null(val_min_disp) && !is.null(val_max_disp))
    {
        # Calculate the ratio between total value range and display value range.
        ratio <- (val_max - val_min) / (val_max_disp - val_min_disp)
        # Calculate total number of colors.
        n_color_total <- round(n_color * ratio)

        # Generate a display color palette.
        color_grad_disp_idx_beg <- round(n_color_total * (val_min_disp - val_min) / (val_max - val_min)) + 1
        if(color_grad_disp_idx_beg < 1) color_grad_disp_idx_beg <- 1
        if(color_grad_disp_idx_beg > n_color_total) color_grad_disp_idx_beg <- n_color_total
        color_grad_disp_idx_end <- round(n_color_total * (val_max_disp - val_min) / (val_max - val_min))
        if(color_grad_disp_idx_end < 1) color_grad_disp_idx_end <- 1
        if(color_grad_disp_idx_end > n_color_total) color_grad_disp_idx_end <- n_color_total

        # Set the color map to display.
        color_grad_total <- color_pal(n_color_total)
        color_grad_disp <- color_grad_total[color_grad_disp_idx_beg:color_grad_disp_idx_end]
    }
    else
    {
        # Set the color map to display.
        color_grad_disp <- color_pal(n_color)
    }


    # Test the color codes.
    if(test)
    {
        print("Color Values:")
        print(color_grad_disp)
        main <- paste("Color Gradient from", paste0(pal,collapse=", "))
        image(1:n_color, 1, as.matrix(1:n_color), col=color_grad_disp, xlab="Gradient", ylab="", main=main, xaxt="n", yaxt="n", bty="n")
    }

    # Return the color set.
    return(color_grad_disp)
}
