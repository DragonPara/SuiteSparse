#[=======================================================================[.rst:
FindGraphBLAS
--------

The following copyright and license applies to just this file only, not to
the GraphBLAS library itself:
LAGraph, (c) 2019-2022 by The LAGraph Contributors, All Rights Reserved.
SPDX-License-Identifier: BSD-2-Clause
See additional acknowledgments in the LICENSE file,
or contact permission@sei.cmu.edu for the full terms.

Find the native GraphBLAS includes and library.

IMPORTED Targets
^^^^^^^^^^^^^^^^

This module defines :prop_tgt:`IMPORTED` target ``GraphBLAS::GraphBLAS``, if
GraphBLAS has been found.

Result Variables
^^^^^^^^^^^^^^^^

This module defines the following variables:

::

  GRAPHBLAS_INCLUDE_DIR    - where to find GraphBLAS.h, etc.
  GRAPHBLAS_LIBRARY        - dynamic GraphBLAS library
  GRAPHBLAS_STATIC         - static GraphBLAS library
  GRAPHBLAS_LIBRARIES      - List of libraries when using GraphBLAS.
  GRAPHBLAS_FOUND          - True if GraphBLAS found.

::

Hints
^^^^^

A user may set ``GRAPHBLAS_ROOT`` or ``GraphBLAS_ROOT`` to a GraphBLAS
installation root to tell this module where to look.

Otherwise, the first place searched is in ../GraphBLAS, relative to the LAGraph
source directory.  That is, if GraphBLAS and LAGraph reside in the same parent
folder, side-by-side, and if it contains GraphBLAS/Include/GraphBLAS.h file and
GraphBLAS/build/libgraphblas.so (or dylib, etc), then that version is used.
This takes precedence over the system-wide installation of GraphBLAS, which
might be an older version.  This method gives the user the ability to compile
LAGraph with their own copy of GraphBLAS, ignoring the system-wide version.

If SuiteSparse:GraphBLAS is the GraphBLAS library being utilized,
all the Find*.cmake files in SuiteSparse are installed by 'make install' into
/usr/local/lib/cmake/SuiteSparse (where '/usr/local' is the
${CMAKE_INSTALL_PREFIX}).  To access this file, place the following commands
in your CMakeLists.txt file.  See also SuiteSparse/Example/CMakeLists.txt:

    set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
        ${CMAKE_INSTALL_PREFIX}/lib/cmake/SuiteSparse )

#]=======================================================================]

# NB: this is built around assumptions about one particular GraphBLAS
# installation (SuiteSparse:GraphBLAS). As other installations become available
# changes to this will likely be required.


## New versions of SuiteSparse GraphBLAS (8.0.3 and newer) ##

find_package ( GraphBLAS ${GraphBLAS_FIND_VERSION} CONFIG
    PATHS ${CMAKE_BINARY_DIR} ${PROJECT_SOURCE_DIR}/../GraphBLAS/build NO_DEFAULT_PATH )
if ( NOT TARGET SuiteSparse::GraphBLAS )
    find_package ( GraphBLAS ${GraphBLAS_FIND_VERSION} CONFIG )
endif ( )

if ( GraphBLAS_FOUND )
    if ( TARGET SuiteSparse::GraphBLAS )
        # It's not possible to create an alias of an alias.
        get_property ( _graphblas_aliased TARGET SuiteSparse::GraphBLAS
            PROPERTY ALIASED_TARGET )
        if ( "${_graphblas_aliased}" STREQUAL "" )
            add_library ( GraphBLAS::GraphBLAS ALIAS SuiteSparse::GraphBLAS )
        else ( )
            add_library ( GraphBLAS::GraphBLAS ALIAS ${_graphblas_aliased} )
        endif ( )
    endif ( )
    if ( TARGET SuiteSparse::GraphBLAS_static )
        # It's not possible to create an alias of an alias.
        get_property ( _graphblas_aliased TARGET SuiteSparse::GraphBLAS_static
            PROPERTY ALIASED_TARGET )
        if ( "${_graphblas_aliased}" STREQUAL "" )
            add_library ( GraphBLAS::GraphBLAS_static ALIAS SuiteSparse::GraphBLAS_static )
        else ( )
            add_library ( GraphBLAS::GraphBLAS_static ALIAS ${_graphblas_aliased} )
        endif ( )
    endif ( )
    return ( )
endif ( )


## Older versions of SuiteSparse GraphBLAS (or different vendor?) ##

# "Include" for SuiteSparse:GraphBLAS
find_path ( GRAPHBLAS_INCLUDE_DIR
  NAMES GraphBLAS.h
  HINTS ${GRAPHBLAS_ROOT}
  HINTS ENV GRAPHBLAS_ROOT
  HINTS ${PROJECT_SOURCE_DIR}/..
  HINTS ${PROJECT_SOURCE_DIR}/../GraphBLAS
  HINTS ${PROJECT_SOURCE_DIR}/../SuiteSparse/GraphBLAS
  PATH_SUFFIXES include Include
  )

# dynamic SuiteSparse:GraphBLAS library
find_library ( GRAPHBLAS_LIBRARY
  NAMES graphblas
  HINTS ${GRAPHBLAS_ROOT}
  HINTS ENV GRAPHBLAS_ROOT
  HINTS ${PROJECT_SOURCE_DIR}/..
  HINTS ${PROJECT_SOURCE_DIR}/../GraphBLAS
  HINTS ${PROJECT_SOURCE_DIR}/../SuiteSparse/GraphBLAS
  PATH_SUFFIXES lib build alternative
  )

if ( MSVC )
    set ( STATIC_NAME graphblas_static )
else ( )
    set ( STATIC_NAME graphblas )
endif ( )

# static SuiteSparse:GraphBLAS library
set ( save ${CMAKE_FIND_LIBRARY_SUFFIXES} )
set ( CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX} )
find_library ( GRAPHBLAS_STATIC
  NAMES ${STATIC_NAME}
  HINTS ${GRAPHBLAS_ROOT}
  HINTS ENV GRAPHBLAS_ROOT
  HINTS ${PROJECT_SOURCE_DIR}/..
  HINTS ${PROJECT_SOURCE_DIR}/../GraphBLAS
  HINTS ${PROJECT_SOURCE_DIR}/../SuiteSparse/GraphBLAS
  PATH_SUFFIXES lib build alternative
  )
set ( CMAKE_FIND_LIBRARY_SUFFIXES ${save} )
if ( MINGW AND GRAPHBLAS_STATIC MATCHES ".*\.dll\.a" )
    set ( GRAPHBLAS_STATIC "" )
endif ( )

# get version of the library from the dynamic library name
get_filename_component ( GRAPHBLAS_LIBRARY  ${GRAPHBLAS_LIBRARY} REALPATH )
get_filename_component ( GRAPHBLAS_FILENAME ${GRAPHBLAS_LIBRARY} NAME )
string (
    REGEX MATCH "[0-9]+.[0-9]+.[0-9]+"
    GRAPHBLAS_VERSION
    ${GRAPHBLAS_FILENAME}
  )

if ( GRAPHBLAS_VERSION )
    if ( ${GRAPHBLAS_VERSION} MATCHES "([0-9]+).[0-9]+.[0-9]+" )
        set ( GraphBLAS_VERSION_MAJOR ${CMAKE_MATCH_1} )
    endif ( )
    if ( ${GRAPHBLAS_VERSION} MATCHES "[0-9]+.([0-9]+).[0-9]+" )
        set ( GraphBLAS_VERSION_MINOR ${CMAKE_MATCH_1} )
    endif ( )
    if ( ${GRAPHBLAS_VERSION} MATCHES "[0-9]+.[0-9]+.([0-9]+)" )
        set ( GraphBLAS_VERSION_PATCH ${CMAKE_MATCH_1} )
    endif ( )
    message ( STATUS "major: ${GraphBLAS_VERSION_MAJOR}" )
    message ( STATUS "minor: ${GraphBLAS_VERSION_MINOR}" )
    message ( STATUS "patch: ${GraphBLAS_VERSION_PATCH}" )
endif ( )

# set ( GRAPHBLAS_VERSION "" )
if ( EXISTS "${GRAPHBLAS_INCLUDE_DIR}" AND NOT GRAPHBLAS_VERSION )
    # if the version does not appear in the filename, read the include file
    file ( STRINGS ${GRAPHBLAS_INCLUDE_DIR}/GraphBLAS.h GRAPHBLAS_MAJOR_STR
        REGEX "define GxB_IMPLEMENTATION_MAJOR" )
    file ( STRINGS ${GRAPHBLAS_INCLUDE_DIR}/GraphBLAS.h GRAPHBLAS_MINOR_STR
        REGEX "define GxB_IMPLEMENTATION_MINOR" )
    file ( STRINGS ${GRAPHBLAS_INCLUDE_DIR}/GraphBLAS.h GRAPHBLAS_PATCH_STR
        REGEX "define GxB_IMPLEMENTATION_SUB" )
    message ( STATUS "major: ${GRAPHBLAS_MAJOR_STR}" )
    message ( STATUS "minor: ${GRAPHBLAS_MINOR_STR}" )
    message ( STATUS "patch: ${GRAPHBLAS_PATCH_STR}" )
    string ( REGEX MATCH "[0-9]+" GraphBLAS_VERSION_MAJOR ${GRAPHBLAS_MAJOR_STR} )
    string ( REGEX MATCH "[0-9]+" GraphBLAS_VERSION_MINOR ${GRAPHBLAS_MINOR_STR} )
    string ( REGEX MATCH "[0-9]+" GraphBLAS_VERSION_PATCH ${GRAPHBLAS_PATCH_STR} )
    set (GRAPHBLAS_VERSION "${GraphBLAS_VERSION_MAJOR}.${GraphBLAS_VERSION_MINOR}.${GraphBLAS_VERSION_PATCH}")
endif ( )

set ( GRAPHBLAS_LIBRARIES ${GRAPHBLAS_LIBRARY} )

include ( FindPackageHandleStandardArgs )

find_package_handle_standard_args(
  GraphBLAS
  REQUIRED_VARS GRAPHBLAS_LIBRARIES GRAPHBLAS_INCLUDE_DIR
  VERSION_VAR GRAPHBLAS_VERSION
  )

mark_as_advanced(
  GRAPHBLAS_INCLUDE_DIR
  GRAPHBLAS_LIBRARY
  GRAPHBLAS_STATIC
  GRAPHBLAS_LIBRARIES
  )

if ( GRAPHBLAS_FOUND )
    message ( STATUS "GraphBLAS version: ${GRAPHBLAS_VERSION}" )
    message ( STATUS "GraphBLAS include: ${GRAPHBLAS_INCLUDE_DIR}" )
    message ( STATUS "GraphBLAS library: ${GRAPHBLAS_LIBRARY}" )
    message ( STATUS "GraphBLAS static:  ${GRAPHBLAS_STATIC}" )
else ( )
    message ( STATUS "GraphBLAS not found" )
    set ( GRAPHBLAS_INCLUDE_DIR "" )
    set ( GRAPHBLAS_LIBRARIES "" )
    set ( GRAPHBLAS_LIBRARY "" )
    set ( GRAPHBLAS_STATIC "" )
endif ( )

# Create target from information found
if ( GRAPHBLAS_LIBRARY )
    add_library ( GraphBLAS::GraphBLAS UNKNOWN IMPORTED )
    set_target_properties ( GraphBLAS::GraphBLAS PROPERTIES
        IMPORTED_LOCATION "${GRAPHBLAS_LIBRARY}"
        IMPORTED_INCLUDE_DIRECTORIES "${GRAPHBLAS_INCLUDE_DIR}" )
endif ( )
if ( GRAPHBLAS_STATIC )
    add_library ( GraphBLAS::GraphBLAS_static UNKNOWN IMPORTED )
    set_target_properties ( GraphBLAS::GraphBLAS_static PROPERTIES
        IMPORTED_LOCATION "${GRAPHBLAS_STATIC}"
        IMPORTED_INCLUDE_DIRECTORIES "${GRAPHBLAS_INCLUDE_DIR}" )
endif ( )