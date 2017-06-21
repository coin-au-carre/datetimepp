cmake_minimum_required(VERSION 2.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
include_directories("." "date")

add_subdirectory(examples)

enable_testing()
add_subdirectory(test)
