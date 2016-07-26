# Build library common
add_library(common common.cc)

# Install library and header files
install(TARGETS common DESTINATION bin/common)
FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES ${HEADER_FILES} DESTINATION include/common)