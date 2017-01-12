cmake_minimum_required(VERSION 2.8.12)
project(archivepp C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

include(CTest)
include(CheckCXXCompilerFlag)

if (NOT WIN32)
	include(CodeCoverage)
endif ()

option(ENABLE_COVERAGE "Enables coverage report for the library.")

CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if (NOT COMPILER_SUPPORTS_CXX11)
    message(FATAL_ERROR "The current compiler, ${CMAKE_CXX_COMPILER}, does not support C++ 11.")
endif ()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
    # Microsoft Visual Studio
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    # G++
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wpedantic")
endif ()

add_subdirectory(lib)
add_subdirectory(tests)
