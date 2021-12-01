# hts library

# The project name
project(High-Throughput-Sequencing)

add_library(hts STATIC
	src/CompositedDGEIlluminaFASTQSequence.cpp
	include/hts/CompositedDGEIlluminaFASTQSequence.hpp
	include/hts/CompositedDGEIlluminaFASTQSequenceGroups.hpp
	include/hts/ConvIlluminaFASTQFile.hpp
	src/ConvIlluminaFASTQSequence.cpp
	include/hts/ConvIlluminaFASTQSequence.hpp
	include/hts/ConvIlluminaFASTQSequenceDemuxer.hpp
	include/hts/DGEIlluminaFASTQFile.hpp
	src/DGEIlluminaFASTQSequence.cpp
	include/hts/DGEIlluminaFASTQSequence.hpp
	include/hts/DGEIlluminaFASTQSequenceDemuxer.hpp
	src/FASTQFileGroupOutputStreams.cpp
	include/hts/FASTQFileGroupOutputStreams.hpp
	include/hts/FASTQFileReader.hpp
	src/FASTQSequence.cpp
	include/hts/FASTQSequence.hpp
	include/hts/FASTQSequenceDemuxController.hpp
	include/hts/FASTQSequenceDemuxer.hpp
	include/hts/FASTQSequenceGroups.hpp
	include/hts/FASTQSequencePipe.hpp
	src/IlluminaFASTQSequence.cpp
	include/hts/IlluminaFASTQSequence.hpp
	include/hts/PairedConvIlluminaFASTQSequence.hpp
	include/hts/PairedConvIlluminaFASTQSequenceGroups.hpp
	include/hts/PairedDGEIlluminaFASTQSequence.hpp
	src/PairedFASTQFileGroupOutputStreams.cpp
	include/hts/PairedFASTQFileGroupOutputStreams.hpp
	src/PairedFASTQFilePathReader.cpp
	include/hts/PairedFASTQFilePathReader.hpp
	include/hts/PairedFASTQSequenceCreator.hpp
	include/hts/PairedFASTQSequencePipe.hpp
	include/hts/SAMAlignmentCounter.hpp
	src/SAMAlignmentInfoPrinter.cpp
	include/hts/SAMAlignmentInfoPrinter.hpp
	include/hts/SAMAlignmentLine.hpp
	src/SAMAlignmentMandatoryFields.cpp
	include/hts/SAMAlignmentMandatoryFields.hpp
	src/SAMAlignmentOptionalField.cpp
	include/hts/SAMAlignmentOptionalField.hpp
	src/SAMAlignmentOptionalFields.cpp
	include/hts/SAMAlignmentOptionalFields.hpp
	include/hts/SAMAlignmentPipe.hpp
	src/SAMCompositedDGEIlluminaAlignmentMandatoryFields.cpp
	include/hts/SAMCompositedDGEIlluminaAlignmentMandatoryFields.hpp
	include/hts/SAMCompositedDGEIlluminaSTARFeatureCountsAlignmentLine.hpp
	include/hts/SAMFileReader.hpp
	src/SAMGeneUMIAlignmentCounter.cpp
	include/hts/SAMGeneUMIAlignmentCounter.hpp
	src/SAMHeaderCommentLine.cpp
	include/hts/SAMHeaderCommentLine.hpp
	src/SAMHeaderDataField.cpp
	include/hts/SAMHeaderDataField.hpp
	src/SAMHeaderDataLine.cpp
	include/hts/SAMHeaderDataLine.hpp
	src/SAMHeaderLine.cpp
	include/hts/SAMHeaderLine.hpp
	src/SAMSTARFeatureCountsAlignmentOptionalFields.cpp
	include/hts/SAMSTARFeatureCountsAlignmentOptionalFields.hpp
	src/WellBarcodeReader.cpp
	include/hts/WellBarcodeReader.hpp
	include/hts/WellBarcodeTable.hpp
)

target_include_directories(hts
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(hts
	PUBLIC utk
)

set_target_properties(hts PROPERTIES
	CXX_STANDARD 17
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)
