cmake_minimum_required(VERSION 2.8.12)
project(archivepp C CXX)

include(CTest)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

add_subdirectory(lib)
add_subdirectory(tests)
