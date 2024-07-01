cmake_minimum_required(VERSION 3.14)

set(BUILD_EXAMPLES true)

add_subdirectory(src)

add_subdirectory(examples)

include_directories(src)
