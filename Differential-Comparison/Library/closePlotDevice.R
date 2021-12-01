# Close a plot device.

closePlotDevice <- function(dev_info=NULL, dev_name=NULL, pars=NULL)
{
    # dev_info: information list of plot device, returned by openPlotDevice.
    # dev_name: name of plot device (if dev_info is not specified).
    # pars: graphical parameters of plot device (if dev_info is not specified).

    # Check input device arguments.
    if(!is.null(dev_info))
    {
        dev_name <- dev_info$dev
        pars <- dev_info$pars
    }

    # Restore original graphical parameters.
    if(!is.null(pars)) par(pars)

    # Close specified plot or current plot.
    if(!is.null(dev_name)) dev.off(dev_name)
    else if(!is.null(dev.list())) dev.off()
}
