# Build library reader
add_library(reader reader.cc)

# Build unit tests
set(LIBS common reader gtest)

add_executable(reader_test reader_test.cc)
target_link_libraries(reader_test gtest_main ${LIBS})

# Install library and header files
install(TARGETS reader DESTINATION lib/reader)
FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES ${HEADER_FILES} DESTINATION include/reader)
