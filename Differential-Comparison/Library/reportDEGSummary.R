# Summarize DEG statistics.

reportDEGSummary <- function(deg_diff_stats, deg_counts_file=NULL, verbose=FALSE, func_dir=NULL)
{
    # Load required library
    require(tools)

    # Load user-defined functions.
    if(is.null(func_dir)) func_dir <- getwd()
    source(file.path(func_dir, "sortTableRows.R"), local=TRUE)

    # Check deg_diff_stats argument.
    if(!is.null(deg_diff_stats))
    {
        if(!is.data.frame(deg_diff_stats))
        {
            warning("Input deg_diff_stats must be a data frame!")
            return(NULL)
        }
    }
    else
    {
        warning("deg_diff_stats cannot be NULL!")
        return(NULL)
    }

    # Summarize DEGs.
    deg_diff_stats_summary <- NULL
    if(nrow(deg_diff_stats) > 0)
    {
        # Create a new field to indicate the comparison condition.
        deg_diff_stats_table <- data.frame(deg_diff_stats, Condition=paste(deg_diff_stats$Condition1,deg_diff_stats$Condition2,sep="."), stringsAsFactors=FALSE)
        # Summarize DEG counts of each comparison condition for each combination of cell and plate.
        for(cell_line in sort(unique(deg_diff_stats_table$Cell)))
        {
            deg_diff_stats_table_cell <- deg_diff_stats_table[deg_diff_stats_table$Cell==cell_line,]
            for(plate in sort(unique(deg_diff_stats_table_cell$Plate)))
            {
                deg_diff_stats_table_cell_plate <- deg_diff_stats_table_cell[deg_diff_stats_table_cell$Plate==plate,]
                # Get the counts of DEGs at each drug-treated condition.
                drug_table <- as.data.frame(table(deg_diff_stats_table_cell_plate$Condition), stringsAsFactors=FALSE)
                colnames(drug_table) <- c("Condition", "DEG")
                # Get the names of grouped drugs at their original order.
                cond_names <- unique(deg_diff_stats_table_cell_plate$Condition)
                # Reorder DEG count table according to the original order of grouped drugs.
                drug_table <- sortTableRows(cond_names, drug_table, "Condition")
                deg_diff_stats_summary <- rbind(deg_diff_stats_summary, data.frame(Condition=drug_table$Condition, Cell=rep(cell_line, nrow(drug_table)), Plate=rep(plate, nrow(drug_table)), DEG=drug_table$DEG, stringsAsFactors=FALSE))
            }
        }
        # Remove the Condition field.
        deg_diff_stats_table$Condition <- NULL
        # Sort DEG summary in the order of Drug, Cell, Plate, and DEG.
        deg_diff_stats_summary <- deg_diff_stats_summary[order(xtfrm(deg_diff_stats_summary[,"Condition"]),xtfrm(deg_diff_stats_summary[,"Cell"]),xtfrm(deg_diff_stats_summary[,"Plate"]),xtfrm(deg_diff_stats_summary[,"DEG"])),]
        # Restore the "Condition" field to original "Condition1" and "Condition2" fields.
        conds12 <- data.frame(t(sapply(deg_diff_stats_summary$Condition,function(x){unlist(strsplit(x, " *\\. *"))})), row.names=NULL, stringsAsFactors=FALSE)
        colnames(conds12) <- c("Condition1", "Condition2")
        deg_diff_stats_summary$Condition <- NULL
        deg_diff_stats_summary <- data.frame(conds12, deg_diff_stats_summary, row.names=NULL, stringsAsFactors=FALSE)

        # Print DEG summary.
        if(verbose) print(deg_diff_stats_summary)

        # Prepare file names.
        if(!is.null(deg_counts_file))
        {
            deg_counts_file_base <- basename(deg_counts_file)
            deg_counts_file_dir <- dirname(deg_counts_file)
            deg_counts_file_main_name <- file_path_sans_ext(deg_counts_file_base)
            deg_counts_file_ext_name <- file_ext(deg_counts_file_base)
            # Save the DEG summary file.
            deg_counts_file_name <- paste(deg_counts_file_main_name, deg_counts_file_ext_name, sep=".")
            deg_diff_stats_summary_file <- file.path(deg_counts_file_dir, deg_counts_file_name)
            write.table(deg_diff_stats_summary, deg_diff_stats_summary_file, sep="\t", quote=FALSE, row.names=FALSE)
        }
    }

    # Return the DEG summary.
    return(deg_diff_stats_summary)
}
