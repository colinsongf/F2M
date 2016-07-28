# Build unit tests
set(LIBS common reader gtest)

add_executable(reader_test reader_test.cc)
target_link_libraries(reader_test gtest_main ${LIBS})

add_executable(parser_test parser_test.cc)
target_link_libraries(parser_test gtest_main ${LIBS})

add_executable(linear_algebra_test linear_algebra_test.cc)
target_link_libraries(linear_algebra_test gtest_main ${LIBS})