include_directories(zip_tests PUBLIC ${CMAKE_SOURCE_DIR}/lib/include)
include_directories(zip_tests PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/utf8_v2_3_4/source)

include_directories(${CMAKE_SOURCE_DIR}/thirdparty/doctest)
add_definitions(-DDOCTEST_CONFIG_IMPLEMENT_WITH_MAIN)

if (WIN32)
else ()
    set(ARCHIVEPP_LIBRARY ${CMAKE_SOURCE_DIR}/build/lib/libarchivepp.a)

    add_definitions(-D_UNIX)
endif ()

# Enable flags for code coverage
#set(GCC_COVERAGE_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
#set(GCC_COVERAGE_LINK_FLAGS "-fprofile-arcs -ftest-coverage")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

# --- ZIP ----
add_executable(zip_tests zip_tests.cpp)
target_link_libraries(zip_tests ${ARCHIVEPP_LIBRARY})
#target_link_libraries(zip_tests gcov)

find_package(libzip REQUIRED)
if (LIBZIP_FOUND)
    target_include_directories(zip_tests PUBLIC ${LIBZIP_INCLUDE_DIR})
    target_link_libraries(zip_tests ${LIBZIP_LIBRARY})
endif ()

# --- RAR ---
add_executable(rar_tests rar_tests.cpp)
target_link_libraries(rar_tests ${ARCHIVEPP_LIBRARY})
#target_link_libraries(rar_tests gcov)

find_package(libunrar REQUIRED)
if (LIBUNRAR_FOUND)
    target_include_directories(rar_tests PUBLIC ${LIBUNRAR_INCLUDE_DIR})
    target_link_libraries(rar_tests ${LIBUNRAR_LIBRARY})
endif ()

# --- 7Z --- 

# Declare our test modules depend on libarchivepp
add_dependencies(zip_tests archivepp)
add_dependencies(rar_tests archivepp)

# Create the individual tests
add_test(archivepp_zip_test zip_tests)
add_test(archivepp_rar_test rar_tests)
