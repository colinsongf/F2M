# Build unittests.
set(LIBS common reader gtest)

add_executable(reader_test reader_test.cc)
target_link_libraries(reader_test gtest_main ${LIBS})
