include_directories(${CMAKE_SOURCE_DIR}/thirdparty/utf8_v2_3_4/source)

if (WIN32)
    add_definitions(-DARCHIVEPP_USE_WSTRING)
endif ()

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

set(CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR})
add_library(archivepp STATIC
    ${CURRENT_DIR}/include/archivepp/archive.hpp
    ${CURRENT_DIR}/include/archivepp/basic_archive.hpp

    ${CURRENT_DIR}/include/archivepp/archive_zip.hpp ${CURRENT_DIR}/src/archive_zip.cpp
    ${CURRENT_DIR}/include/archivepp/archive_entry_zip.hpp ${CURRENT_DIR}/src/archive_entry_zip.cpp
)

find_package(libzip REQUIRED)
if (LIBZIP_FOUND)
    target_include_directories(archivepp PUBLIC ${LIBZIP_INCLUDE_DIR})
    target_link_libraries(archivepp ${LIBZIP_LIBRARY})
endif ()

#find_package(libunrar REQUIRED)
#if (LIBUNRAR_FOUND)
#    include_directories(${LIBUNRAR_INCLUDE_DIR})
#    target_link_libraries(archivepp ${LIBUNRAR_LIBRARY})
#endif ()
