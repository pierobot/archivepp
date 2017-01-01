cmake_minimum_required(VERSION 2.8.12)
project(archivepp C CXX)

include(CTest)
include(CheckCXXCompilerFlag)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if (COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")
else ()
    message(FATAL_ERROR "The current compiler, ${CMAKE_CXX_COMPILE_FEATURES}, does not support C++ 11.")
endif ()

add_subdirectory(lib)
add_subdirectory(tests)
