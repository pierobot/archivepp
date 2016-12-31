include_directories(zip_tests PUBLIC ${CMAKE_SOURCE_DIR}/lib/include)
include_directories(zip_tests PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/utf8_v2_3_4/source)

include_directories(${CMAKE_SOURCE_DIR}/thirdparty/doctest)
add_definitions(-DDOCTEST_CONFIG_IMPLEMENT_WITH_MAIN)

set(ARCHIVEPP_LIBRARY ${CMAKE_SOURCE_DIR}/build/lib/libarchivepp.a)

# --- ZIP ----
add_executable(zip_tests zip_tests.cpp)
target_link_libraries(zip_tests ${ARCHIVEPP_LIBRARY})

find_package(libzip REQUIRED)
if (LIBZIP_FOUND)
    target_include_directories(zip_tests PUBLIC ${LIBZIP_INCLUDE_DIR})
    target_link_libraries(zip_tests ${LIBZIP_LIBRARY})
endif ()

# --- RAR ---

# --- 7Z --- 

# Declare our test modules depend on libarchivepp
add_dependencies(zip_tests archivepp)

# Add the individual test modules to a single test
add_test(test zip_tests)