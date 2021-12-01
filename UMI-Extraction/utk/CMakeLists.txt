# utk library

# The project name
project(Universal-Toolkit)

add_library(utk STATIC
	src/DSVReader.cpp
	include/utk/DSVReader.hpp
	src/FileUtils.cpp
	include/utk/FileUtils.hpp
	src/LineReader.cpp
	include/utk/LineReader.hpp
	src/LineWriter.cpp
	include/utk/LineWriter.hpp
	src/ProgramArguments.cpp
	include/utk/ProgramArguments.hpp
	src/StringUtils.cpp
	include/utk/StringUtils.hpp
	src/SystemProperties.cpp
	include/utk/SystemProperties.hpp
)

target_include_directories(utk
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(utk PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)
