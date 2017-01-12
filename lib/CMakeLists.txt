include_directories(${CMAKE_SOURCE_DIR}/thirdparty/utf8_v2_3_4/source)

if (${CMAKE_SIZEOF_VOID_P} STREQUAL "8")
    add_definitions(-DARCHIVEPP_ARCH_64)
endif ()

include(CheckCXXSymbolExists)
if (WIN32)
    #add_definitions(-DZIP_STATIC)
    add_definitions(-DARCHIVEPP_USE_WSTRING)

    CHECK_CXX_SYMBOL_EXISTS(RtlZeroSecureMemory "Windows.h" HAS_RTLZEROSECUREMEMORY)
    if (HAS_RTLZEROSECUREMEMORY)
        add_definitions(-DARCHIVEPP_HAS_RTLZEROSECUREMEMORY)
    endif ()
else ()
    # unrar/dll.hpp has type definitions we need in a #ifdef _UNIX statement
    add_definitions(-D_UNIX)

    CHECK_CXX_SYMBOL_EXISTS(memset_s "string.h" HAS_MEMSET_S)
    if (HAS_MEMSET_S)
        add_definitions(-DARCHIVEPP_HAS_MEMSET_S)
    endif ()
endif ()

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

set(CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR})
add_library(archivepp STATIC
    ${CURRENT_DIR}/include/archivepp/archive.hpp
    ${CURRENT_DIR}/include/archivepp/basic_archive.hpp

    ${CURRENT_DIR}/src/archive_zip.cpp
    ${CURRENT_DIR}/include/archivepp/archive_zip.hpp
    
    ${CURRENT_DIR}/src/archive_entry_zip.cpp
    ${CURRENT_DIR}/include/archivepp/archive_entry_zip.hpp

    ${CURRENT_DIR}/src/archive_rar.cpp
    ${CURRENT_DIR}/include/archivepp/archive_rar.hpp

    ${CURRENT_DIR}/src/archive_entry_rar.cpp
    ${CURRENT_DIR}/include/archivepp/archive_entry_rar.hpp

    ${CURRENT_DIR}/src/string.cpp
    ${CURRENT_DIR}/include/archivepp/string.hpp

    ${CURRENT_DIR}/src/secure_allocator.cpp
    ${CURRENT_DIR}/include/archivepp/secure_allocator.hpp
)

find_package(ZLIB REQUIRED)
if (ZLIB_FOUND)
    target_include_directories(archivepp PUBLIC ${ZLIB_INCLUDE_DIRS})
    target_link_libraries(archivepp ${ZLIB_LIBRARIES})
endif ()

find_package(libzip REQUIRED)
if (LIBZIP_FOUND)
    target_include_directories(archivepp PUBLIC ${LIBZIP_INCLUDE_DIR})
    target_link_libraries(archivepp ${LIBZIP_LIBRARY})
endif ()

find_package(libunrar REQUIRED)
if (LIBUNRAR_FOUND)
    include_directories(${LIBUNRAR_INCLUDE_DIR})
    target_link_libraries(archivepp ${LIBUNRAR_LIBRARY})
endif ()

if (ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_COVERAGE}")
    setup_target_for_coverage(archivepp_coverage ctest coverage)
endif ()