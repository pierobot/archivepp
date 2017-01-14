# This module finds libzip and defines the following variables
# LIBZIP_FOUND
# LIBZIP_LIBRARY
# LIBZIP_INCLUDE_DIRS
# LIBZIP_VERSION

find_path(LIBZIP_INCLUDE_DIR
    NAMES
        zip.h
    HINTS
        $ENV{ProgramFiles}/libzip
    PATH_SUFFIXES
        include
)

find_path(LIBZIP_ZIPCONF_INCLUDE_DIR
    NAMES
        zipconf.h
    HINTS
        $ENV{ProgramFiles}/libzip
    PATH_SUFFIXES
        lib/libzip/include
)

find_library(LIBZIP_LIBRARY
    NAMES
        zip
    HINTS
        $ENV{ProgramFiles}/libzip/lib
)

set(LIBZIP_VERSION 0)
# Read zipconf.h and get version string
# I don't know regex so.... yea.

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    LIBZIP
    REQUIRED_VARS LIBZIP_LIBRARY LIBZIP_INCLUDE_DIR LIBZIP_ZIPCONF_INCLUDE_DIR
#    VERSION_VAR LIBZIP_VERSION
)

#set(LIBZIP_VERSION ${LIBZIP_VERSION} CACHE STRING "Version of libzip")
set(LIBZIP_INCLUDE_DIRS ${LIBZIP_INCLUDE_DIR} ${LIBZIP_ZIPCONF_INCLUDE_DIR})
