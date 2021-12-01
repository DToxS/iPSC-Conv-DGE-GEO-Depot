#!/usr/bin/env Rscript
# This script run comparison of differential expressed molecules for
# genes and proteins.

###################### Define internally used functions ######################

# Determine script invocation method.
getInvokeMethod <- function()
{
    # Get all command arguments including R system arguments.
    cmd_args <- commandArgs(FALSE)

    # Get the value of "--file" option.
    matched_pattern <- regexpr("(?<=^--file=).+", cmd_args, perl=TRUE)
    prog_cmd <- regmatches(cmd_args, matched_pattern)
    n_prog_cmd <- length(prog_cmd)

    # Get the availability of "--args" option.
    args_opt_avail <- "--args" %in% cmd_args

    # Determine invocation method based on n_prog_cmd and args_opt_avail.
    invoke_method <- NULL
    if(n_prog_cmd == 0) invoke_method <- "R"
    else if(n_prog_cmd == 1) invoke_method <- "Rscript"
    else invoke_method <- "unknown"

    # Return invocation method.
    return(invoke_method)
}

# Obtain the path of this program in Rscript command line.
getProgPath <- function(invoke_method)
{
    prog_path <- NULL

    # Determine the function/config path.
    if(invoke_method == "Rscript")
    {
        # Retrieve function path from the path of this main program.
        cmd_args <- commandArgs(FALSE)
        # Get the value of "--file" option.
        matched_pattern <- regexpr("(?<=^--file=).+", cmd_args, perl=TRUE)
        prog_path <- regmatches(cmd_args, matched_pattern)
    }

    # Return the function/config path.
    return(prog_path)
}

# Check the command line of script invocation by Rscript.
checkInvokeCommandLine <- function(invoke_method, n_args_min)
{
    # Initialize checking status.
    cmd_status <- TRUE

    # Only check command line for Rscript invocation.
    if(invoke_method == "Rscript")
    {
        # Get the program name.
        prog_name <- basename(getProgPath(invoke_method))
        # Get the command line of script invocation by Rscript.
        cmd_args <- commandArgs(TRUE)
        if(length(cmd_args) < n_args_min)
        {
            cat(paste("Usage:", prog_name, "[Configuration File] [Repository Directory] [Function Directory]\n"))
            cat("\n")
            cat("Arguments:\n\n")
            cat("\t[Configuration File] is a data file with customizable configuration parameters.\n\n")
            cat("\t[Repository Directory] is a directory with \"Counts\", \"Params\", \"Scripts\", and \"Results\" sub-directories.\n\n")
            cat("\t[Function Directory] is a directory with all rquired R functions (Optional).\n\n")
            cmd_status <- FALSE
        }
    }
    else if(invoke_method == "R")
    {
        # Set cmd_status to TRUE to allow running this script in an interactive R console.
        cmd_status <- TRUE
    }
    else
    {
        warning("This script needs to be launched in either interactive R console or Rscript command line!")
        cmd_status <- FALSE
    }

    # Return checking status.
    return(cmd_status)
}

# Obtain the value of a specified input arguments in Rscript command line or use the default.
getArg <- function(invoke_method, arg_name, arg_pos, default_value=NULL)
{
    # invoke_method: "R" or "Rscript".

    arg <- NULL

    # Determine the function/config path.
    if(invoke_method == "Rscript")
    {
        # Type 2: for the invocation by Rscript at system terminal.
        cmd_args <- commandArgs(TRUE)
        if(length(cmd_args) > arg_pos-1) arg <- cmd_args[arg_pos]
        else
        {
            warning(paste("No", arg_name, "is provided and use the default value!"))
            arg <- default_value
        }
        if(is.null(arg)) warning(paste(arg_name, "is NULL when invoke_method is \"Rscript\"!"))
    }
    else if(invoke_method == "R")
    {
        # Type 1: for the invocation by "R" at R terminal.
        arg <- default_value
        if(is.null(arg)) warning(paste(arg_name, "is NULL when invoke_method is \"R\"!"))
    }
    else warning("invoke_method must be either \"R\" or \"Rscript\"!")

    # Return the function/config path.
    return(arg)
}

# Set default directories for differential comparison analysis.
setDefaultDEMDirs <- function(top_dir=Sys.getenv("HOME"), dataset_name="default")
{
    # Initialize default directories.
    lincs_dir <- file.path(top_dir, "LINCSData")
    prog_dir <- file.path(lincs_dir, "Programs")
    default_func_dir <- file.path(prog_dir, "Library")
    dataset_dir <- file.path(lincs_dir, "Datasets")
    diff_dataset_dir <- file.path(dataset_dir, "Difference")
    lincs_diff_dataset_dir <- file.path(diff_dataset_dir, "LINCS.Dataset")

    # Customize default directories for specific datasets.

    # RNAseq Plate 1
    if(dataset_name == "LINCS.Dataset.Gene.LINCS.20150409")
    {
        # RNA sequencing 20150409
        default_repo_dir <- file.path(lincs_diff_dataset_dir, dataset_name)
        default_config_dir <- file.path(default_repo_dir, "Configs")
        default_config_file <- paste("Configs", dataset_name, "tsv", sep=".")
        default_config_file <- file.path(default_config_dir, default_config_file)
    }
    # RNAseq Plate 2
    else if(dataset_name == "LINCS.Dataset.Gene.LINCS.20150503")
    {
        # RNA sequencing 20150503
        default_repo_dir <- file.path(lincs_diff_dataset_dir, dataset_name)
        default_config_dir <- file.path(default_repo_dir, "Configs")
        default_config_file <- paste("Configs", dataset_name, "tsv", sep=".")
        default_config_file <- file.path(default_config_dir, default_config_file)
    }
    # RNAseq Plate 3
    else if(dataset_name == "LINCS.Dataset.Gene.LINCS.20150712")
    {
        # RNA sequencing 20150712
        default_repo_dir <- file.path(lincs_diff_dataset_dir, dataset_name)
        default_config_dir <- file.path(default_repo_dir, "Configs")
        default_config_file <- paste("Configs", dataset_name, "tsv", sep=".")
        default_config_file <- file.path(default_config_dir, default_config_file)
    }
    # RNAseq Plate 4
    else if(dataset_name == "LINCS.Dataset.Gene.LINCS-PC.20151120")
    {
        # RNA sequencing 20151120
        default_repo_dir <- file.path(lincs_diff_dataset_dir, dataset_name)
        default_config_dir <- file.path(default_repo_dir, "Configs")
        default_config_file <- paste("Configs", dataset_name, "tsv", sep=".")
        default_config_file <- file.path(default_config_dir, default_config_file)
    }
    # Conventional RNAseq
    else if(dataset_name == "LINCS.Dataset.Gene.LINCS.20151223")
    {
        # RNA sequencing 20151223
        default_repo_dir <- file.path(lincs_diff_dataset_dir, dataset_name)
        default_config_dir <- file.path(default_repo_dir, "Configs")
        default_config_file <- paste("Configs", dataset_name, "tsv", sep=".")
        default_config_file <- file.path(default_config_dir, default_config_file)
    }
    # RNAseq Plate 5: correlation-based clustered samples
    else if(dataset_name == "LINCS.Dataset.Gene.LINCS-Cor.20161108")
    {
        # RNA sequencing 20161108
        dataset_name_dir <- gsub("-Cor","",dataset_name)
        default_repo_dir <- file.path(lincs_diff_dataset_dir, dataset_name_dir)
        default_config_dir <- file.path(default_repo_dir, "Configs")
        default_config_file <- paste("Configs", dataset_name, "tsv", sep=".")
        default_config_file <- file.path(default_config_dir, default_config_file)
    }
    # RNAseq Plate 5: specified samples
    else if(dataset_name == "LINCS.Dataset.Gene.LINCS-Spec.20161108")
    {
        # RNA sequencing 20161108
        dataset_name_dir <- gsub("-Spec","",dataset_name)
        default_repo_dir <- file.path(lincs_diff_dataset_dir, dataset_name_dir)
        default_config_dir <- file.path(default_repo_dir, "Configs")
        default_config_file <- paste("Configs", dataset_name, "tsv", sep=".")
        default_config_file <- file.path(default_config_dir, default_config_file)
    }
    # RNAseq Plate 6
    else if(dataset_name == "LINCS.Dataset.Gene.LINCS.20170710")
    {
        # RNA sequencing 20170710
        default_repo_dir <- file.path(lincs_diff_dataset_dir, dataset_name)
        default_config_dir <- file.path(default_repo_dir, "Configs")
        default_config_file <- paste("Configs", dataset_name, "tsv", sep=".")
        default_config_file <- file.path(default_config_dir, default_config_file)
    }
    # RNAseq UMI for method paper.
    else if(dataset_name == "Method-Paper-UMI")
    {
        # RNA sequencing UMI
        default_repo_dir <- file.path(lincs_diff_dataset_dir, "Method-Paper", "UMI")
        default_config_dir <- file.path(default_repo_dir, "Configs")
        default_config_file <- paste("Configs.LINCS.Dataset.Gene.LINCS", "UMI", "tsv", sep=".")
        default_config_file <- file.path(default_config_dir, default_config_file)
    }
    # RNAseq Conv for method paper.
    else if(dataset_name == "Method-Paper-Conv")
    {
        # RNA sequencing Conv
        default_repo_dir <- file.path(lincs_diff_dataset_dir, "Method-Paper", "Conv")
        default_config_dir <- file.path(default_repo_dir, "Configs")
        default_config_file <- paste("Configs.LINCS.Dataset.Gene.LINCS", "Conv", "tsv", sep=".")
        default_config_file <- file.path(default_config_dir, default_config_file)
    }
    # RNAseq CTRL in both Conv and UMI for method paper.
    else if(dataset_name == "Method-Paper-Both-All")
    {
        # RNA sequencing Conv
        default_repo_dir <- file.path(lincs_diff_dataset_dir, "Method-Paper", "Both", "All")
        default_config_dir <- file.path(default_repo_dir, "Configs")
        default_config_file <- paste("Configs.LINCS.Dataset.Gene.LINCS", "All", "tsv", sep=".")
        default_config_file <- file.path(default_config_dir, default_config_file)
    }
    # RNAseq CTRL in both Conv and UMI for method paper.
    else if(dataset_name == "Method-Paper-Both-CTRL")
    {
        # RNA sequencing Conv
        default_repo_dir <- file.path(lincs_diff_dataset_dir, "Method-Paper", "Both", "CTRL")
        default_config_dir <- file.path(default_repo_dir, "Configs")
        default_config_file <- paste("Configs.LINCS.Dataset.Gene.LINCS", "CTRL", "tsv", sep=".")
        default_config_file <- file.path(default_config_dir, default_config_file)
    }
    # RNAseq SOR in both Conv and UMI for method paper.
    else if(dataset_name == "Method-Paper-Both-SOR")
    {
        # RNA sequencing Conv
        default_repo_dir <- file.path(lincs_diff_dataset_dir, "Method-Paper", "Both", "SOR")
        default_config_dir <- file.path(default_repo_dir, "Configs")
        default_config_file <- paste("Configs.LINCS.Dataset.Gene.LINCS", "SOR", "tsv", sep=".")
        default_config_file <- file.path(default_config_dir, default_config_file)
    }
    # RNAseq SUN in both Conv and UMI for method paper.
    else if(dataset_name == "Method-Paper-Both-SUN")
    {
        # RNA sequencing Conv
        default_repo_dir <- file.path(lincs_diff_dataset_dir, "Method-Paper", "Both", "SUN")
        default_config_dir <- file.path(default_repo_dir, "Configs")
        default_config_file <- paste("Configs.LINCS.Dataset.Gene.LINCS", "SUN", "tsv", sep=".")
        default_config_file <- file.path(default_config_dir, default_config_file)
    }
    # PRTseq Plate 1
    else if(dataset_name == "LINCS.Dataset.Protein.LINCS.20151015")
    {
        # Protein spectrum 20151015
        default_repo_dir <- file.path(lincs_diff_dataset_dir, dataset_name)
        default_config_dir <- file.path(default_repo_dir, "Configs")
        default_config_file <- paste("Configs", dataset_name, "tsv", sep=".")
        default_config_file <- file.path(default_config_dir, default_config_file)
    }
    # PRTseq Plate 2
    else if(dataset_name == "LINCS.Dataset.Protein.LINCS.20160212")
    {
        # Protein spectrum 20160212
        default_repo_dir <- file.path(lincs_diff_dataset_dir, dataset_name)
        default_config_dir <- file.path(default_repo_dir, "Configs")
        default_config_file <- paste("Configs", dataset_name, "tsv", sep=".")
        default_config_file <- file.path(default_config_dir, default_config_file)
    }
    # PRTseq Plate 4: 24 raw samples.
    else if(dataset_name == "LINCS.Dataset.Protein.LINCS.20161012")
    {
        # Protein spectrum 20160212
        default_repo_dir <- file.path(lincs_diff_dataset_dir, dataset_name)
        default_config_dir <- file.path(default_repo_dir, "Configs")
        default_config_file <- paste("Configs", dataset_name, "tsv", sep=".")
        default_config_file <- file.path(default_config_dir, default_config_file)
    }
    else if(dataset_name == "Lior_TGFb")
    {
        default_repo_dir <- file.path(diff_dataset_dir, "Lior_TGFb.Dataset")
        default_config_dir <- file.path(default_repo_dir, "Configs")
        default_config_file <- "Configs.Lior_TGFb.tsv"
        default_config_file <- file.path(default_config_dir, default_config_file)
    }
    else if(dataset_name == "default")
    {
        default_func_dir <- file.path(top_dir, "LINCS/Programs")
        default_repo_dir <- file.path(top_dir, "LINCS/Dataset")
        default_config_dir <- file.path(default_repo_dir, "Configs")
        default_config_file <- "Configs.tsv"
        default_config_file <- file.path(default_config_dir, default_config_file)
    }
    else
    {
        default_func_dir <- NULL
        default_repo_dir <- NULL
        default_config_file <- NULL
    }

    # Copy all variables from current function's environment to its parent's environment.
    for(obj in ls()) assign(obj, get(obj,environment()), parent.env(environment()))
}

################### Set up basic computational environment ###################

# Initialize default directories.
# Set default directories for differential comparison analysis.
setDefaultDEMDirs("LINCS.Dataset.Gene.LINCS.20170710")

# Determine script invocation method.
invoke_method <- getInvokeMethod()
if(invoke_method == "unknown") stop("Script must be invoked by either R or Rscript!")

# Obtain invoked R script command.
stopifnot(checkInvokeCommandLine(invoke_method,2))

# Determine customizable configuration file.
config_file <- getArg(invoke_method=invoke_method, arg_name="config_file", arg_pos=1, default_value=default_config_file)
stopifnot(!is.null(config_file) && length(config_file)==1 && is.character(config_file))
if(!file.exists(config_file)) stop(paste0("Config file \"", config_file, "\" doesn't exist!"))
config_file_base <- basename(config_file)
config_file_dir <- normalizePath(dirname(config_file))
config_file <- file.path(config_file_dir, config_file_base)
# Determine directory path of data repository.
repo_dir <- getArg(invoke_method=invoke_method, arg_name="repo_dir", arg_pos=2, default_value=default_repo_dir)
stopifnot(!is.null(repo_dir) && length(repo_dir)==1 && is.character(repo_dir))
if(!dir.exists(repo_dir)) stop(paste0("Repository directory \"", repo_dir, "\" doesn't exist!"))
repo_dir <- normalizePath(repo_dir)
# Determine directory path of all required R function scripts.
func_dir <- getArg(invoke_method=invoke_method, arg_name="func_dir", arg_pos=3, default_value=default_func_dir)
stopifnot(!is.null(func_dir) && length(func_dir)==1 && is.character(func_dir))
if(!dir.exists(func_dir)) stop(paste0("Function directory \"", func_dir, "\" doesn't exist!"))
func_dir <- normalizePath(func_dir)

# Load the function of differential expression comparison.
source(file.path(func_dir, "compareMoleculeExpression.R"), local=TRUE)

# Run the analysis of differential expression comparison.
read_counts_datasets <- compareMoleculeExpression(func_dir=func_dir, repo_dir=repo_dir, config_file=config_file)
