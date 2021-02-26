CMAKE_MINIMUM_REQUIRED(VERSION 3.15 FATAL_ERROR)

PROJECT(ThirdParty)

INCLUDE(ExternalProject)

# cflags

SET(cflags_ROOT       ${CMAKE_BINARY_DIR}/cflags)
SET(cflags_SOURCE_DIR ${CMAKE_BINARY_DIR}/cflags_src)

FIND_PACKAGE(cflags CONFIG QUIET)

IF(NOT cflags_FOUND)
    ExternalProject_Add(
        ThirdParty_cflags
        GIT_REPOSITORY      "https://github.com/WhoBrokeTheBuild/cflags.git"
        GIT_TAG             "main"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          ${cflags_SOURCE_DIR}
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${cflags_ROOT}
        BUILD_COMMAND       ""
        UPDATE_COMMAND      ""
    )
ENDIF()

# clog

SET(clog_ROOT       ${CMAKE_BINARY_DIR}/clog)
SET(clog_SOURCE_DIR ${CMAKE_BINARY_DIR}/clog_src)

FIND_PACKAGE(clog CONFIG QUIET)

IF(NOT clog_FOUND)
    ExternalProject_Add(
        ThirdParty_clog
        GIT_REPOSITORY      "https://github.com/WhoBrokeTheBuild/clog.git"
        GIT_TAG             "main"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          ${clog_SOURCE_DIR}
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${clog_ROOT}
        BUILD_COMMAND       ""
        UPDATE_COMMAND      ""
    )
ENDIF()
