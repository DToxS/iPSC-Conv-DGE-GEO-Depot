#!/usr/bin/env bash
# This script compares the difference of gene expressions for Conv dataset.

# Initialize error code.
EXIT_CODE=0

# Set top directory.
DATASET_DIR="${HOME}/LINCSData/Datasets/Alignment/LINCS.Dataset/iPSC-CM/Conv-GEO-Depot"
PROG_DIR="${DATASET_DIR}/Programs/Differential-Comparison"

# Set executable program.
PROG_FILE_PATH="${PROG_DIR}/Compare-Expr-Diffs.GEO.sh"
ANLYS_PROG_FILE_PATH="${PROG_DIR}/Compare-Molecule-Expression.R"
FUNC_DIR="${PROG_DIR}/Library"

# Set input arguments.
REPO_DIR="${DATASET_DIR}"
CONFIG_FILE_PATH="${REPO_DIR}/Configs/Conv-RNAseq-Configs.tsv"

# Launch the differential comparison anlaysis.
if [ -d "${REPO_DIR}" ]; then
	RESULTS_DIR="${REPO_DIR}/Results"
	# To avoid overwriting any exisiting data, run the DEG anlaysis only
	# if no results directory already exists.
	if [ ! -d "${RESULTS_DIR}" ]; then
		mkdir "${RESULTS_DIR}"
		EXIT_CODE=$?
		if [ ${EXIT_CODE} -eq 0 ]; then
			"${PROG_FILE_PATH}" "${ANLYS_PROG_FILE_PATH}" "${CONFIG_FILE_PATH}" "${REPO_DIR}" "${FUNC_DIR}"
			EXIT_CODE=$?
		else
			echo "ERROR: cannot create the result directory ${RESULTS_DIR}!" 1>&2
		fi
	else
		echo "ERROR: the results directory ${RESULTS_DIR} already exists!" 1>&2
		EXIT_CODE=1
	fi
else
	echo "ERROR: the dataset directory ${REPO_DIR} is not found!" 1>&2
	EXIT_CODE=1
fi

# Exit with error code.
exit ${EXIT_CODE}
