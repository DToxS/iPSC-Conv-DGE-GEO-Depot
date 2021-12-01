# SAM-Alignment-Counter executable

# The project name
project(SAM-Alignment-Counter)

add_executable(SAM-Alignment-Counter
	SAM-Alignment-Counter.cpp
	SAMAlignmentCounterArguments.cpp
	SAMAlignmentCounterArguments.hpp
)

target_include_directories(SAM-Alignment-Counter
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SAM-Alignment-Counter
	PRIVATE hts
	PRIVATE utk
)

set_target_properties(SAM-Alignment-Counter PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)
