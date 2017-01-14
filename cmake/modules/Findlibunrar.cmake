# This module finds libunrar and defines the following variables
# LIBUNRAR_FOUND
# LIBUNRAR_LIBRARY
# LIBUNRAR_INCLUDE_DIR
# LIBUNRAR_VERSION

find_path(LIBUNRAR_INCLUDE_DIR
    NAMES
        unrar/dll.hpp
    HINTS
        $ENV{ProgramFiles}/unrar
    PATH_SUFFIXES
        include
)

find_path(LIBUNRAR_VERSION_INCLUDE_DIR
    NAMES
        unrar/version.hpp
    HINTS
        $ENV{ProgramFiles}/unrar
    PATH_SUFFIXES
        include
)

find_library(LIBUNRAR_LIBRARY 
    NAMES
        unrar
    HINTS
        $ENV{ProgramFiles}/unrar
)

set(LIBUNRAR_VERSION 0)
# Read unrar/version.h and get version string
# I don't know regex so.... yea.

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(
    LIBUNRAR
    REQUIRED_VARS LIBUNRAR_LIBRARY LIBUNRAR_INCLUDE_DIR
#    VERSION_VAR LIBUNRAR_VERSION
)

set(LIBUNRAR_VERSION ${LIBUNRAR_VERSION} CACHE STRING "Version of libunrar")
