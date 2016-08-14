# Build library loss
add_library(loss logit_loss.cc)

# Install library and header files
install(TARGETS loss DESTINATION bin/loss)
FILE(GLOB HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES ${HEADER_FILES} DESTINATION include/loss)