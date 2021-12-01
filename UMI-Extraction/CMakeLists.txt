cmake_minimum_required(VERSION 3.8)

# The project name
project(UMI-Extraction)

# Check compiler option: native
include(CheckCXXCompilerFlag)

# GCC/Clang
set(GCCLANG_NATIVE_ARCH_OPT "-march=native")
CHECK_CXX_COMPILER_FLAG(${GCCLANG_NATIVE_ARCH_OPT} GCCLANG_NATIVE_ARCH_SPT)
# ICC
set(ICC_NATIVE_ARCH_OPT_UNIX "-xHost")
CHECK_CXX_COMPILER_FLAG(${ICC_NATIVE_ARCH_OPT_UNIX} ICC_NATIVE_ARCH_SPT_UNIX)
set(ICC_NATIVE_ARCH_OPT_WIN "/QxHost")
CHECK_CXX_COMPILER_FLAG(${ICC_NATIVE_ARCH_OPT_WIN} ICC_NATIVE_ARCH_SPT_WIN)

# Set compiler option: native
if(GCCLANG_NATIVE_ARCH_SPT)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCCLANG_NATIVE_ARCH_OPT}")
elseif(ICC_NATIVE_ARCH_SPT_UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ICC_NATIVE_ARCH_OPT_UNIX}")
elseif(ICC_NATIVE_ARCH_SPT_WIN)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ICC_NATIVE_ARCH_OPT_WIN}")
endif()

# Set compiling options
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Add directory structure
add_subdirectory(utk)
add_subdirectory(hts)
add_subdirectory(SAM-Alignment-Counter)
