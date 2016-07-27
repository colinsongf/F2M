# Build library reader
add_library(reader reader.cc)

# Install library and header files
install(TARGETS reader DESTINATION lib/reader)
FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES ${HEADER_FILES} DESTINATION include/reader)