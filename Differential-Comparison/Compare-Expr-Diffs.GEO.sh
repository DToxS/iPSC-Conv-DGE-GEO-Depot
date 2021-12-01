#!/usr/bin/env bash
# This script calls an R program to compare the difference of gene expressions
# at multiple drug treatment conditions.

#
# Function definitions
#

help_msg ()
{
	echo "Usage: "$1" [Analysis Program] [Config File] [Repo Dir] [Func Dir]" 1>&2
	echo "       [Analysis Program]: the path of the R analysis program Compare-Molecule-Expression.R" 1>&2
	echo "       [Config File]: a configuration file for differential comparison analysis" 1>&2
	echo "       [Repo Dir]: a top directory for Counts, Params, Scripts, and Results sub-directories" 1>&2
	echo "       [Func Dir]: the directory containing all R functions rquired by the R analysis program" 1>&2
	return 1
}

check_dir ()
{
	if [ ! -d "$1" ]; then
		echo "ERROR: Directory "$1" is not found!" 1>&2
		return 1
	else
		return 0
	fi
}

check_file ()
{
	if [[ ! -f "$1" && ! -h "$1" ]] || [[ ! -r "$1" ]]; then
		echo "ERROR: File "$1" is not found or not accessible!" 1>&2
		return 1
	else
		return 0
	fi
}

#
# Main program
#

# Initialize error code.
EXIT_CODE=0

# Obtain and check program path.
PROG_PATH="$0"
PROG_DIR="$(dirname "${PROG_PATH}")"
PROG_NAME="$(basename "${PROG_PATH}")"

# Process input arguments.
if [ ${EXIT_CODE} -eq 0 ]; then
	# Specify the range of the number of input arguments.
	N_ARGS_MIN=4
	N_ARGS_MAX=4
	# Check and assign input arguments.
	if [ $# -ge "${N_ARGS_MIN}" ] && [ $# -le "${N_ARGS_MAX}" ]; then
		if [ ${EXIT_CODE} -eq 0 ] && [ $# -ge $((N_ARGS_MIN)) ] && [ "${N_ARGS_MIN}" -ge 1 ]; then
			ANLYS_PROG_FILE="${@:((N_ARGS_MIN-3)):1}"
			CONFIG_FILE="${@:((N_ARGS_MIN-2)):1}"
			REPO_DIR="${@:((N_ARGS_MIN-1)):1}"
			FUNC_DIR="${@:((N_ARGS_MIN)):1}"
			check_file "${ANLYS_PROG_FILE}" && \
				check_file "${CONFIG_FILE}" && \
				check_dir "${REPO_DIR}" && \
				check_dir "${FUNC_DIR}"
			EXIT_CODE=$?
		fi
	else
		# Print help message for the wrong number of input arguments.
		help_msg "${PROG_NAME}"
		EXIT_CODE=$?
	fi
fi

# Launch the R program for differential comparison.
if [ ${EXIT_CODE} -eq 0 ]; then
	Rscript "${ANLYS_PROG_FILE}" "${CONFIG_FILE}" "${REPO_DIR}" "${FUNC_DIR}"
	EXIT_CODE=$?
fi

# Exit with error code.
exit ${EXIT_CODE}
