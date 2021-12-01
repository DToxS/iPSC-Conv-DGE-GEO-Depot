# Convert a experiment design table to standard experiment design fields
# specified by std_exprt_design_field_names.

convertExperimentDesignTable <- function(exprt_design, field_names_positions, field_types=c(Species="character", Subject="character", State="character", Culture="character", Replicate="character", Measure="character", Sample="character", Time="character"), std_exprt_design_field_names=c("Species", "Subject", "State", "Culture", "Replicate", "Measure", "Sample", "Time"), species_default="Human", subject_default="A", state_default="CTRL", culture_default=1, replicate_default=1, measure_default=1, sample_default=1:nrow(exprt_design), time_default=48, legal_chars=NULL, repl_chars=NULL, compress_space=TRUE, func_dir=NULL)
{
    # field_names_positions includes column indices of specified experiment
    # desgin fields in given experiment design table, in a form of:
    # c(Species=1, Subject=2, State=3, Culture=4, Replicate=5, Measure=6, Sample=7, Time=8)

    # Load user-defined functions.
    if(is.null(func_dir)) func_dir <- getwd()
    source(file.path(func_dir, "replaceIllegalChars.R"), local=TRUE)

    # Verify the given field names is contained in standard experiment design fields.
    field_names <- names(field_names_positions)
    stopifnot(all(field_names %in% std_exprt_design_field_names))
    stopifnot(all(names(field_types) %in% std_exprt_design_field_names))

    # Generate a experiment design table with default values.
    n_samples <- nrow(exprt_design)
    exprt_design_table <- data.frame(rep(species_default,n_samples), rep(subject_default,n_samples), rep(state_default,n_samples), rep(culture_default,n_samples), rep(replicate_default,n_samples), rep(measure_default,n_samples), Sample=sample_default, rep(time_default,n_samples), stringsAsFactors=FALSE)
    colnames(exprt_design_table) <- std_exprt_design_field_names
    stopifnot(setequal(colnames(exprt_design_table), std_exprt_design_field_names))

    # Assign specified fields of given experiment design to the table.
    for(field_idx in 1:length(field_names_positions))  exprt_design_table[,field_names[field_idx]] <- exprt_design[,field_names_positions[field_idx]]

    # Attach the fields other than std_exprt_design_field_names to exprt_design_table.
    exprt_design_table <- cbind(exprt_design_table, exprt_design[,!(1:ncol(exprt_design) %in% field_names_positions),drop=FALSE])

    # Convert each field of experiment design table to specified data type.
    if(!is.null(field_types))
    {
        for(field_name in std_exprt_design_field_names)
        {
            field_type <- field_types[field_name]
            if(field_type == "character")
            {
                exprt_design_table[,field_name] <- as.character(exprt_design_table[,field_name])
                # Replcae pre-defined illegal characters in all character fields with specified character.
                if(!is.null(legal_chars) && is.character(legal_chars) && nchar(legal_chars)>0) exprt_design_table[,field_name] <- replaceIllegalChars(string=exprt_design_table[,field_name], legal_chars=legal_chars, repl_chars=repl_chars, compress_space=compress_space)
            }
            else if(field_type == "numeric") exprt_design_table[,field_name] <- as.numeric(exprt_design_table[,field_name])
            else if(field_type == "logical") exprt_design_table[,field_name] <- as.logical(exprt_design_table[,field_name])
            else warning(paste(field_type, "isn't a supported field type!"))
        }
    }
    else warning("Skip field type conversion as field_type isn't specified!")

    # Return converted experiment design table.
    return(exprt_design_table)
}
