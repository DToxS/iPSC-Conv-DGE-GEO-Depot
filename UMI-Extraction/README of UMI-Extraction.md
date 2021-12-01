# Extracting Sequence Reads Tagged with Unique UMI Barcodes

This computational program takes the detailed alignment files in the SAM format reported by *featureCounts* from the 3'-DGE mRNA sequencing data, identifies and removes the sequence reads tagged with duplicate UMI barcodes, and create a sequence alignment file composed of solely the reads tagged with unique UMI barcodes.

## Development Software

The development software needed for compiling this program includes:

- *Bash* shell in Unix-like operating systems (e.g. Linux, MacOS, Unix)
- *GCC* (>= v7) or *Clang* (>= v5) C++ compiler
- *CMake* (>= v3.8)

## Program Compilation

After `Program-Source-Codes.tar.gz` is extracted into the `Programs` directory in the <u>Data Preparation</u> step in [Analyzing 3'-DGE Random-Primed mRNA-Sequencing Data](https://github.com/DToxS/DGE-GEO-Depot), the source codes of this program will be placed in the directory `UMI-Extraction`. This program is written in C++ programming language and needs to be compiled into a binary executable file using *GCC* or *Clang* compiler that supports C++17 features. Since the source codes have been configured with the *CMake* build system, it becomes quite simple to build the source codes in the following a few steps:

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

Now this custom built program is ready to be used by the shell scripts in the [Feature Counts](https://github.com/DToxS/Feature-Counts) analysis.

## Usage Command

The command line to run the `SAM-Alignment-Counter` program is the following:

```
Usage: SAM-Alignment-Counter [Input SAM File]
       [Input SAM File]: an input SAM file reported by featureCounts from STAR's alignment results.
       [Output SAM File]: an output SAM file containing unique sequence alignments tagged with unique UMI barcodes. 
```

