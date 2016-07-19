# Build library common.
add_library(common logging.cc)

# Build unittests
set(LIBS common gtest)

add_executable(carray_test carray_test.cc)
target_link_libraries(carray_test gtest_main ${LIBS})

add_executable(sparse_vector_test sparse_vector_test.cc)
target_link_libraries(sparse_vector_test gtest_main ${LIBS})

add_executable(dense_vector_test dense_vector_test.cc)
target_link_libraries(dense_vector_test gtest_main ${LIBS})

# Install library and header files
install(TARGETS common DESTINATION lib/common)
FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES ${HEADER_FILES} DESTINATION include/common)
