# Filter outlier samples by removing outlier samples.

filterGeneExpSamples <- function(exprt_design_merged, read_counts_merged, drug_names, dist_cutoffs, subset_field_name=NULL, dist_cutoff_outlier=0.01, dist_cutoff_group=0.015, min_samples=3, filter_outlier=TRUE, keep_under_samples=FALSE, plot_orig_clust=FALSE, plot_filter_clust=FALSE, plot_pass_clust=FALSE, plot_empty_clust=FALSE, color_sample_groups=TRUE, group_colors=NULL, color_field_pos=3, color_field_type="numeric", hline_color="deeppink", hline_type=4, hline_width=1, clust_title_elems_names=c(State="State",Culture="Culture",Measurement="Measurement",Subject="Subject"), clust_title_elems_flags=c(State=TRUE,Culture=TRUE,Measurement=TRUE,Subject=TRUE), clust_title_elems_keys=c("State", "Culture", "Measurement", "Subject"), title_cex=1.75, branch_line_width=2, leaf_lab_cex=0.75, ylab_cex=1.5, leg_title="Culture", leg_pos="topright", leg_cex=1, leg_title_col="black", leg_title_adj=1, verbose=FALSE, func_dir=NULL)
{
    # Load required library
    require("matrixStats")

    # Load user-defined functions.
    if(is.null(func_dir)) func_dir <- getwd()

    source(file.path(func_dir, "getCutoff.R"), local=TRUE)
    source(file.path(func_dir, "filterGeneExpSamplesSubset.R"), local=TRUE)

    # Check input arguments.

    # Check subset_field_name.
    if(!is.null(subset_field_name) && !all(subset_field_name %in% colnames(exprt_design_merged)))
    {
        warning("subset_field_name must be one of the column names of experiment design table!")
        return(NULL)
    }

    # The title text for sample cluster plots is composed of:
    #
    # - the number of samples
    # - the name of treatment state
    # - the name of cell culture experiment
    # - the name of measurement plate
    # - the name of cell subject
    #
    # in the form of:
    #
    # [#] [State] in [Culture] [Measurement] [Subject]
    #
    # e.g. 4 TRS in Exp 15 Assay 6 iPSC B
    if(!is.null(clust_title_elems_names) && !is.null(clust_title_elems_flags))
    {
        if(!(is.character(clust_title_elems_names) && is.vector(clust_title_elems_names) && length(clust_title_elems_names)==length(clust_title_elems_keys)))
        {
            warning(paste0("clust_title_elems_names must be a vector of character strings with a length of ", length(clust_title_elems_keys), "!"))
            return(exprt_design_merged)
        }
        if(names(clust_title_elems_names) != clust_title_elems_keys)
        {
            warning(paste0("The names of clust_title_elems_names must be ", paste0(clust_title_elems_keys,collapse=", "), "!"))
            return(exprt_design_merged)
        }
        if(!(is.logical(clust_title_elems_flags) && is.vector(clust_title_elems_flags) && length(clust_title_elems_flags)==length(clust_title_elems_keys)))
        {
            warning(paste0("clust_title_elems_flags must be a vector of logical values with a length of ", length(clust_title_elems_keys), "!"))
            return(exprt_design_merged)
        }
        if(names(clust_title_elems_flags) != clust_title_elems_keys)
        {
            warning(paste0("The names of clust_title_elems_flags must be ", paste0(clust_title_elems_keys,collapse=", "), "!"))
            return(exprt_design_merged)
        }
    }
    else
    {
        warning("Neither of clust_title_elems_names and clust_title_elems_flags can be NULL!")
        return(exprt_design_merged)
    }

    # Retrieve subset names from experiment design table if a subset
    # category is specified.
    # Note: the name of a subset category must be one of the column
    # names of experiment design table.
    if(!is.null(subset_field_name)) subset_names <- exprt_design_merged[,subset_field_name]
    else subset_names <- NULL

    # Sort and filter sample replicates in each drug-treated condition for
    # all drug groups and cell lines.
    exprt_design_merged_sorted <- NULL
    for(drug_name in drug_names)
    {
        sample_drug_flags <- exprt_design_merged$State==drug_name
        exprt_design_merged_drug <- exprt_design_merged[sample_drug_flags,,drop=FALSE]
        if(!is.null(subset_names)) subset_names_drug <- subset_names[sample_drug_flags]
        else subset_names_drug <- NULL
        for(cell_line in sort(unique(exprt_design_merged$Cell)))
        {
            sample_drug_cell_flags <- exprt_design_merged_drug$Cell==cell_line
            exprt_design_merged_drug_cell <- exprt_design_merged_drug[sample_drug_cell_flags,,drop=FALSE]
            if(!is.null(subset_names_drug)) subset_names_drug_cell <- subset_names_drug[sample_drug_cell_flags]
            else subset_names_drug_cell <- NULL
            for(plate in sort(unique(exprt_design_merged_drug_cell$Plate)))
            {
                # Prepar the dataset of current condition.
                sample_drug_cell_plate_flags <- exprt_design_merged_drug_cell$Plate==plate
                exprt_design_merged_drug_cell_plate <- exprt_design_merged_drug_cell[sample_drug_cell_plate_flags,,drop=FALSE]
                sample_names_drug_cell_plate <- exprt_design_merged_drug_cell_plate$ID
                read_counts_drug_cell_plate <- read_counts_merged[,sample_names_drug_cell_plate,drop=FALSE]
                if(is.matrix(read_counts_drug_cell_plate)) read_counts_drug_cell_plate <- read_counts_drug_cell_plate[!rowAnys(is.na(read_counts_drug_cell_plate)),,drop=FALSE]
                else read_counts_drug_cell_plate <- read_counts_drug_cell_plate[!is.na(read_counts_drug_cell_plate)]
                if(!is.null(subset_names_drug_cell)) subset_names_drug_cell_plate <- subset_names_drug_cell[sample_drug_cell_plate_flags]
                else subset_names_drug_cell_plate <- NULL

                # Calculate the cutoff line for outlier samples.
                # Set cutoff values for outlier samples.
                cutoff <- getCutoff(state=drug_name, cell=cell_line, plate=plate, cutoffs=dist_cutoffs, single=dist_cutoff_outlier, group=dist_cutoff_group)
                if(is.matrix(read_counts_drug_cell_plate) && ncol(read_counts_drug_cell_plate)>min_samples) hline <- cutoff[1]
                else hline <- cutoff[2]

                # Filter outlier samples for each subset of samples at current drug, cell and plate.
                if(!is.null(subset_names_drug_cell_plate))
                {
                    exprt_design_merged_drug_cell_plate_filtered <- NULL
                    for(subset_name in sort(unique(subset_names_drug_cell_plate)))
                    {
                        # Prepar the dataset of current condition.
                        sample_drug_cell_plate_subset_flags <- subset_names_drug_cell_plate==subset_name
                        exprt_design_merged_drug_cell_plate_subset <- exprt_design_merged_drug_cell_plate[sample_drug_cell_plate_subset_flags,,drop=FALSE]
                        sample_names_drug_cell_plate_subset <- exprt_design_merged_drug_cell_plate_subset$ID
                        read_counts_drug_cell_plate_subset <- read_counts_drug_cell_plate[,sample_names_drug_cell_plate_subset,drop=FALSE]

                        # Set title text for sample cluster plots.
                        clust_title_elems_values <- c(State=drug_name, Culture=subset_name, Measurement=plate, Subject=cell_line)
                        clust_title_elems <- trimws(paste(clust_title_elems_names, clust_title_elems_values))
                        names(clust_title_elems) <- names(clust_title_elems_names)
                        clust_title_elems <- clust_title_elems[clust_title_elems_flags]
                        if("State"%in%names(clust_title_elems)) clust_title <- paste(clust_title_elems["State"], "in", paste0(clust_title_elems[names(clust_title_elems)!="State"],collapse=" "))
                        else clust_title <- paste0(clust_title_elems[names(clust_title_elems)!="State"],collapse=" ")

                        # Filter outlier samples for current subset of samples.
                        exprt_design_merged_drug_cell_plate_subset_filtered <- filterGeneExpSamplesSubset(subset_name=subset_name, exprt_design=exprt_design_merged_drug_cell_plate_subset, read_counts=read_counts_drug_cell_plate_subset, cutoff=cutoff, hline=hline, dist_cutoff_outlier=dist_cutoff_outlier, dist_cutoff_group=dist_cutoff_group, min_samples=min_samples, filter_outlier=filter_outlier, keep_under_samples=keep_under_samples, plot_orig_clust=plot_orig_clust, plot_filter_clust=plot_filter_clust, plot_pass_clust=plot_pass_clust, plot_empty_clust=plot_empty_clust, color_sample_groups=color_sample_groups, group_colors=group_colors, color_field_pos=color_field_pos, color_field_type=color_field_type, hline_color=hline_color, hline_type=hline_type, hline_width=hline_width, clust_title=clust_title, title_cex=title_cex, branch_line_width=branch_line_width, leaf_lab_cex=leaf_lab_cex, ylab_cex=ylab_cex, leg_title=leg_title, leg_pos=leg_pos, leg_cex=leg_cex, leg_title_col=leg_title_col, leg_title_adj=leg_title_adj, verbose=verbose, func_dir=func_dir)

                        # Save filtered samples for current subset of samples.
                        if(nrow(exprt_design_merged_drug_cell_plate_subset_filtered)>0) exprt_design_merged_drug_cell_plate_filtered <- rbind(exprt_design_merged_drug_cell_plate_filtered, exprt_design_merged_drug_cell_plate_subset_filtered)
                    }
                }
                else
                {
                    # Set title text for sample cluster plots.
                    clust_title_elems_values <- c(State=drug_name, Culture="", Measurement=plate, Subject=cell_line)
                    clust_title_elems <- trimws(paste(clust_title_elems_names, clust_title_elems_values))
                    names(clust_title_elems) <- names(clust_title_elems_names)
                    clust_title_elems <- clust_title_elems[clust_title_elems_flags]
                    if("State"%in%names(clust_title_elems)) clust_title <- paste(clust_title_elems["State"], "in", paste0(clust_title_elems[names(clust_title_elems)!="State"],collapse=" "))
                    else clust_title <- paste0(clust_title_elems[names(clust_title_elems)!="State"],collapse=" ")

                    # Filter outlier samples for all the samples at current drug, cell and plate.
                    exprt_design_merged_drug_cell_plate_filtered <- filterGeneExpSamplesSubset(exprt_design=exprt_design_merged_drug_cell_plate, read_counts=read_counts_drug_cell_plate, cutoff=cutoff, hline=hline, dist_cutoff_outlier=dist_cutoff_outlier, dist_cutoff_group=dist_cutoff_group, min_samples=min_samples, filter_outlier=filter_outlier, keep_under_samples=keep_under_samples, plot_orig_clust=plot_orig_clust, plot_filter_clust=plot_filter_clust, plot_pass_clust=plot_pass_clust, plot_empty_clust=plot_empty_clust, color_sample_groups=color_sample_groups, group_colors=group_colors, color_field_pos=color_field_pos, color_field_type=color_field_type, hline_color=hline_color, hline_type=hline_type, hline_width=hline_width, clust_title=clust_title, title_cex=title_cex, branch_line_width=branch_line_width, leaf_lab_cex=leaf_lab_cex, ylab_cex=ylab_cex, leg_title=leg_title, leg_pos=leg_pos, leg_cex=leg_cex, leg_title_col=leg_title_col, leg_title_adj=leg_title_adj, verbose=verbose, func_dir=func_dir)
                }

                # Save filtered samples at current drug, cell and plate.
                if(nrow(exprt_design_merged_drug_cell_plate_filtered)>0) exprt_design_merged_sorted <- rbind(exprt_design_merged_sorted, exprt_design_merged_drug_cell_plate_filtered)
            }
        }
    }

    # Return filtered experiment design table.
    return(exprt_design_merged_sorted)
}
