cmake_minimum_required(VERSION 2.6)
project(gl)

set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

find_package(glfw)
add_subdirectory(src)
