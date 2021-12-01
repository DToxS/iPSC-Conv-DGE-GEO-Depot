# Analyzing Conventional and DGE mRNA-Sequencing Data for Drug-Treated iPSC Cardiomyocyte Samples

This is a computational pipeline designed for analyzing the raw *FASTQ* data files generated from both the conventional random-primed and the 3'-digital gene expression (3'-DGE) mRNA-sequencing methods and generating a set of differentially expressed genes (DEGs) for each drug treatment and each cell line.

**Note**: the `[Type]` tag used in the following descriptions refers to either `Conv` for conventional random-primed sequence data or `DGE` for 3'-DGE sequence data.

## Software Requirements

The computational pipeline requires the following software programs:

- *Bash* shell in Unix-like operating systems (e.g. Linux, MacOS, Unix)
- *GCC* (>= v7) or *Clang* (>= v5) C++ compiler
- *CMake* (>= v3.8)
- *STAR* (>= v2.5.3a)
- *featureCounts* (from *Subread* >= v1.6.2)
- *R* (>= v3.0.0) with the following packages:
    - *cluster*
    - *edgeR*
    - *gplots*
    - *graphics*
    - *matrixStats*
    - *methods*
    - *RColorBrewer*
    - *stats*
    - *tools*

## Program Compilation

This computational program takes the detailed alignment files in the SAM format reported by *featureCounts* from the 3'-DGE mRNA sequencing data, identifies and removes the sequence reads tagged with duplicate UMI barcodes, and create a sequence alignment file composed of solely the reads tagged with unique UMI barcodes.

### Compiling Codes

After `Program-Source-Codes.tar.gz` is extracted into the `Programs` directory described in the **Data Preparation** step, the source codes of this program will be placed in the directory `UMI-Extraction`. This program is written in C++ programming language and needs to be compiled into a binary executable file using *GCC* or *Clang* compiler that supports C++17 features. Since the source codes have been configured with the *CMake* build system, it becomes quite simple to build the source codes in the following a few steps:

```bash
# Create a build directory.
mkdir $HOME/Build-UMI-Extraction
cd $HOME/Build-UMI-Extraction
# Create the build configuration files.
cmake -DCMAKE_BUILD_TYPE=Release [UMI-Extraction Source-Code Directory]
# Build the binary excecutable.
cmake --build .
```

Once the compilation is finished, a binary executable file `SAM-Alignment-Counter` will be generated in the `SAM-Alignment-Counter` sub-directory under `$HOME/Build-UMI-Extraction`. To be able to run this program, just copy it to any location that is in the search path of the system. For example, the following command copy the program to a system directory `/usr/local/bin`:

```bash
cp $HOME/Build-UMI-Extraction/SAM-Alignment-Counter/SAM-Alignment-Counter /usr/local/bin/
```

Now this custom built program is ready to be used by the shell scripts in the **Feature Counts** step.

### Usage Command

The command line to run the `SAM-Alignment-Counter` program is the following:

```
Usage: SAM-Alignment-Counter [Input SAM File]
       [Input SAM File]: an input SAM file reported by featureCounts from STAR's alignment results.
       [Output SAM File]: an output SAM file containing unique sequence alignments tagged with unique UMI barcodes. 
```

## Data Preparation

The computational pipeline requires a list of input datasets and a tree structure of data directories.

### Data List

The input datasets for the computational pipeline include:

- A set of compressed mRNA-sequencing *FASTQ* data files (`*.fastq.gz`)
- The experiment design file `[Type]-RNAseq-Experiment-Design.tsv.gz`
- The *UCSC* human genome reference library `Reference-Library.tar.gz`
- The data analysis programs `Program-Source-Codes.tar.gz`
- The configuration file for the **differential comparison** step `[Type]-Differential-Comparison-Configs.tar.gz`
- The parameter files for the **differential comparison** step `[Type]-Differential-Comparison-Params.tar.gz`

### Data Layout

Before launching the computational pipeline, a directory tree with corresponding data files needs to be created in the following steps:

1. Create a top directory `[Type]-GEO-Depot` with the following tree structure:

    ```
    [Type]-GEO-Depot
    ├── Seqs
    ├── Aligns
    ├── Counts
    ├── Configs
    ├── Params
    ├── Scripts
    ├── References
    └── Programs
    ```

2. Place all gzipped *FASTQ* data files in the `Seqs` directory. 

3. Extract the following compressed data files into corresponding directories:

    - Extract `[Type]-RNAseq-Experiment-Design.tsv.gz` into the `Counts` directory.
    - Extract `Reference-Library.tar.gz` into the `References` directory.
    - Extract `Program-Source-Codes.tar.gz` into the `Programs` directory and compile the included computational program `UMI-Extraction` as instructed in the [UMI Extraction](https://github.com/DToxS/UMI-Extraction) section.
    - Extract `[Type]-Differential-Comparison-Configs.tar.gz` into the `Configs` directory.
    - Extract `[Type]-Differential-Comparison-Params.tar.gz` into the `Params` directory.

## Analysis Workflow

The workflow of this computational pipeline for analyzing the conventional mRNA-sequence data includes the following steps:

### Sequence Alignment

This step of analysis uses the *STAR* sequence aligner to map all the sequence reads contained in the *FASTQ* data files obtained from high-throughput mRNA-sequencing assay to the UCSC human genome reference library (*hg38*), such that those uniquely aligned reads can be assigned to corresponding reference genes in the **Feature Counts** step later.

#### Inputs

The inputs of this step include:

- The gzipped *FASTQ* data files in the `Seqs` sub-directory of each dataset directory under the `[Type]-GEO-Depot` top directory.
- The UCSC human genome reference library in the `References` directory under the `[Type]-GEO-Depot` top directory.

#### Procedure

The procedure of this step includes the following steps:

1. Set the variable `DATASET_DIR` in `Programs/Sequence-Alignment/Build-[Type]-STAR-Genome-Index.GEO.sh` to the absolute path of the `[Type]-GEO-Depot` top directory, and launch the program to build a set of index files for the human genome reference library used by *STAR*.
2. Set the variable `DATASET_DIR` in `Programs/Sequence-Alignment/Align-[Type]-RNAseq-Reads.GEO.sh` to the absolute path of the `[Type]-GEO-Depot` top directory, and launch the program to align corresponding `[Type]` of mRNA-sequencing *FASTQ* data files to the human genome reference library.

#### Outputs

The outputs of this step include:

- The *STAR* genome index data directory `STAR-Index-[Version]-[Type]` in the `References` directory.
- A set of sequence alignment data files (`*.bam`) in the `Aligns` sub-directory of each dataset directory under the corresponding `[Type]-GEO-Depot` top directory.

### Feature Counts

This step of analysis uses the *featureCounts* program from the *Subread* software to assign all the sequence reads uniquely aligned by the *STAR* aligner in the **Sequence Alignment** step to their corresponding reference genes for each sequenced sample.

#### Inputs

The inputs of this step include:

- A set of sequence alignment data files (`*.bam`) in the `Aligns` sub-directory of each dataset directory under the `[Type]-GEO-Depot` top directory.
- The annotation file of UCSC human genome reference library `RefSeq.hg38.gtf` in the `References` directory under the `[Type]-GEO-Depot` top directory.

#### Procedure

The procedure of this step includes the following steps:

1. Set the variable `DATASET_DIR` in `Programs/Feature-Counts/Count-[Type]-RNAseq-Reads.GEO.sh` to the absolute path of the `[Type]-GEO-Depot` top directory, and launch the program to count the number of uniquely aligned sequence reads mapped to each reference gene.
2. Set the variable `dataset_dir` in the *R* program `Convert-[Type]-RNAseq-Reads.GEO.R` to the absolute path of the `[Type]-GEO-Depot` top directory, and launch the program with the command `Rscript Programs/Feature-Counts/Convert-[Type]-RNAseq-Reads.GEO.R` to convert the gene read-counts table generated by the *featureCounts* program to the format of the input data file used by the **Differential Comparison** step.

#### Outputs

The outputs of this step include:

- A single table `[Type]-RNAseq-Read-Counts.tsv` in the `Counts` directory under the corresponding `[Type]-GEO-Depot` top directory, which contain the uniquely aligned sequence reads for all reference genes and all sequenced samples.

### Differential Comparison

This step of analysis performs a statistical comparison of the difference in the expression level of each human reference genes represented as the counts of uniquely aligned sequence reads between a drug-treated condition and the control condition. At a given false discovery rate (FDR), such comparison generates a set of differentially expressed genes (DEGs) induced by a drug treatment to a sample and is carried out for all the drugs and all the cell lines.

#### Inputs

The inputs of this step include:

- The gene read-counts table `[Type]-RNAseq-Read-Counts.tsv` in the `Counts` sub-directory under the corresponding `[Type]-GEO-Depot` top directory.
- The experiment design table `[Type]-RNAseq-Experiment-Design.tsv` in the `Counts` sub-directory under the corresponding `[Type]-GEO-Depot` top directory.
- The configuration file `[Type]-RNAseq-Configs.tsv` in the `Configs` sub-directory under the corresponding `[Type]-GEO-Depot` top directory.
- The parameter files `[Type]-RNAseq-Drug-Groups.tsv` and `[Type]-RNAseq-Outlier-Cutoffs.tsv` in the `Params` sub-directory under the corresponding `[Type]-GEO-Depot` top directory.

#### Procedure

The procedure of this step includes the following steps:

1. Set the variable `DATASET_DIR` in `Programs/Differential-Comparison/Compare-[Type]-Expr-Diffs.GEO.sh` to the absolute path of the `[Type]-GEO-Depot` top directory, and launch the program to compare the gene expression levels and identify the DEGs at each drug-treated conditions in each cell line.

#### Outputs

The outputs of this step include:

- A single set or multiple sets of statistical comparison result files for the DEGs identified at all drug treatments and in all cell lines with a file name pattern of `Human-[Cell Line]-Hour-48-Plate-[Plate Number]-Calc.CTRL-[Drug Name].tsv`. The single set of DEG statistical results is generated from a single `Conv` sequence dataset and located at the `Results/FDR-[Value]` directory under the `[Type]-GEO-Depot` top directory.

## Result Data

- A table of uniquely aligned sequence reads for all reference genes and all sequenced samples `[Type]-RNAseq-Read-Counts.tsv` in the `Counts` directory (compressed as `[Type]-RNAseq-Read-Counts.tsv.gz`).
- A set of statistical comparison result files for the DEGs identified at all drug treatments and in all cell lines with a file name pattern of `Human-[Cell Line]-Hour-48-Plate-[Plate Number]-Calc.CTRL-[Drug Name].tsv`, located in the `Results/FDR-[Value]` directory under the `[Type]-GEO-Depot` top directory.
