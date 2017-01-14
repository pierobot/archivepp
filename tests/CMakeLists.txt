include_directories(zip_tests PUBLIC ${CMAKE_SOURCE_DIR}/lib/include)
include_directories(zip_tests PUBLIC ${CMAKE_SOURCE_DIR}/thirdparty/utf8_v2_3_4/source)

include_directories(${CMAKE_SOURCE_DIR}/thirdparty/doctest)
add_definitions(-DDOCTEST_CONFIG_IMPLEMENT_WITH_MAIN)

if (WIN32)
	add_definitions(-DARCHIVEPP_USE_WSTRING)
else ()
    add_definitions(-D_UNIX)
endif ()

# --- ZIP ----
add_executable(zip_tests zip_tests.cpp)
target_link_libraries(zip_tests archivepp)

find_package(ZLIB REQUIRED)
if (ZLIB_FOUND)
    target_include_directories(zip_tests PUBLIC ${ZLIB_INCLUDE_DIRS})
    target_link_libraries(zip_tests ${ZLIB_LIBRARIES})
else ()
    message(FATAL_ERROR "Could not find zlib.")
endif ()

find_package(libzip REQUIRED)
if (LIBZIP_FOUND)
    target_include_directories(zip_tests PUBLIC ${LIBZIP_INCLUDE_DIRS})
    target_link_libraries(zip_tests ${LIBZIP_LIBRARY})
else ()
    message(FATAL_ERROR "Could not find libzip.")
endif ()

# --- RAR ---
add_executable(rar_tests rar_tests.cpp)
target_link_libraries(rar_tests archivepp)

find_package(libunrar REQUIRED)
if (LIBUNRAR_FOUND)
    target_include_directories(rar_tests PUBLIC ${LIBUNRAR_INCLUDE_DIR})
    target_link_libraries(rar_tests ${LIBUNRAR_LIBRARY})
else ()
    message(FATAL_ERROR "Could not find libunrar")
endif ()

# --- Miscellaneous ---
add_executable(misc_tests misc_tests.cpp)
target_link_libraries(misc_tests archivepp)

# Link the tests against gcov if necessary
if (ENABLE_COVERAGE)
    target_link_libraries(zip_tests gcov)
    target_link_libraries(rar_tests gcov)
    target_link_libraries(misc_tests gcov)
endif()

# Declare our test modules depend on libarchivepp
add_dependencies(zip_tests archivepp)
add_dependencies(rar_tests archivepp)
add_dependencies(misc_tests archivepp)

# Create the individual tests
add_test(archivepp_zip_test zip_tests)
add_test(archivepp_rar_test rar_tests)
add_test(archivepp_misc_test misc_tests)
