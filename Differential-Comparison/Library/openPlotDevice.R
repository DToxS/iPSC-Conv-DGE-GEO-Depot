# Open a new plot device.

openPlotDevice <- function(dev_type=NULL, file_name=NULL, dir_name=NULL, single_file=FALSE, width=NULL, height=NULL, units=NULL, dpi=NULL, pointsize=NULL, res=NA, font_family=NULL, background=NULL)
{
    # dev_type: type of plot device, e.g. "png", "jpeg"/"jpg", "svg", "pdf", and "eps".
    # file_name: main name of output plot file.
    # dir_name: name of output directory.
    # width: width of plotted image [Default: 2400px for PNG/JPEG and 17" for SVG/PDF/EPS].
    # height: height of plotted image [Default: 2400px for PNG/JPEG and 17" for SVG/PDF/EPS].
    # units: unit of image width and height [Default: px for PNG/JPEG and in for SVG/PDF/EPS].
    # dpi: dot per inch of plotted raster image [Default: 300 for PNG/JPEG].
    # pointsize: point size of plotted image [Default: 24 for PNG/JPEG, 12 for SVG/PDF/EPS].
    # res: resolution in point per inch (ppi) of plotted raster image [Default: NA]
    # font_family: name of font family [Default: sans]
    # background: backgroud color for plotted image [Default: "white" for PNG/JPEG and "transparent" for SVG/PDF/EPS].

    # Check input arguments.

    # Check file name.
    if(!is.null(dev_type)) stopifnot(!is.null(file_name))
    # Check directory name.
    if(!is.null(dev_type))
    {
        if(is.null(dir_name)) dir_name <- "."
        else stopifnot(dir.exists(dir_name))
        dir_name <- normalizePath(dir_name)
    }
    # Check device type.
    raster_dev_types <- c("png", "jpg", "jpeg")
    vector_dev_types <- c("svg", "pdf", "eps")
    dev_types <- c(raster_dev_types, vector_dev_types)
    if(!is.null(dev_type))
    {
        dev_type <- tolower(dev_type)
        stopifnot(dev_type %in% dev_types)
    }
    # Check image width and height: they must be either both NULL or both non NULL.
    stopifnot(!xor(is.null(width), is.null(height)))
    # Check image unit: either px or in.
    if(!is.null(units)) stopifnot(units=="px" || units=="in")

    # Set font family.
    if(is.null(font_family)) font_family <- "sans"

    # Open specified plot device.

    if(!is.null(dev_type))
    {
        if(dev_type %in% raster_dev_types)
        {
            # Set default width and height for raster image plot.
            if(is.null(width) || is.null(height))
            {
                width <- 2400
                height <- 2400
            }
            else
            {
                # Convert image size from inch to pixel.
                if(!is.null(units) && units=="in")
                {
                    if(!is.null(dpi))
                    {
                        width <- width * dpi
                        height <- height * dpi
                    }
                    else
                    {
                        width <- width * 300
                        height <- height * 300
                    }
                }
            }

            # Set default background color.
            if(is.null(background)) background <- "white"

            # Set plot file.
            if(single_file) plot_file <- paste(file_name, dev_type, sep=".")
            else plot_file <- paste(file_name, "%03d", dev_type, sep=".")
            plot_file <- file.path(dir_name, plot_file)

            # Set default point size for PNG plot.
            if(is.null(pointsize)) pointsize <- 24

            # Open a raster image device for plotting.
            # Note: setting the background to transparent will make multiple plots
            # superimpose one after another.
            if(dev_type == "jpg") graph_dev <- "jpeg"
            else graph_dev <- dev_type
            get(graph_dev)(filename=plot_file, width=width, height=height, units="px", pointsize=pointsize, res=res, family=font_family, bg=background)
        }
        else if(dev_type %in% vector_dev_types)
        {
            # Set default width and height for SVG plot.
            if(is.null(width) || is.null(height))
            {
                width <- 17
                height <- 17
            }
            else
            {
                # Convert image size from pixel to inch.
                if(!is.null(units) && units=="px")
                {
                    if(!is.null(dpi))
                    {
                        width <- round(width / dpi)
                        height <- round(height / dpi)
                    }
                    else
                    {
                        width <- round(width / 300)
                        height <- round(height / 300)
                    }
                }
            }

            # Set default background color.
            if(is.null(background)) background <- "transparent"

            # Set plot file.
            if(single_file) plot_file <- paste(file_name, dev_type, sep=".")
            else plot_file <- paste(file_name, "%03d", dev_type, sep=".")
            plot_file <- file.path(dir_name, plot_file)

            # Set default point size for PDF plot.
            if(is.null(pointsize)) pointsize <- 12

            # Open a EPS device for plotting.
            if(dev_type == "eps")
            {
                graph_dev <- "postscript"
                setEPS()
            }
            else graph_dev <- dev_type
            # Open a vector image device for plotting.
            get(graph_dev)(file=plot_file, width=width, height=height, pointsize=pointsize, family=font_family, bg=background, onefile=single_file)
        }
        else stop(paste0("dev_type must be one of ", paste0(dev_types,collapse=", "), "!"))
    }
    else
    {
        # Plot to screen.
        dev.new()
    }

    # Create a set of device info to return.

    # Get the name of current plot device.
    cur_dev <- dev.cur()
    # Save the graphical parameters of current plot device.
    orig_pars <- par(no.readonly=TRUE)
    # Create a list.
    dev_info <- list(dev=cur_dev, pars=orig_pars)

    # Return the device info.
    return(dev_info)
}
