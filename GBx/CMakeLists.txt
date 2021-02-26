
INCLUDE(SetSourceGroups)

FILE(
    GLOB_RECURSE
    _sources
    "Public/*.h"
    "Private/*.h"
    "Private/*.c"
)

FILE(
    GLOB_RECURSE
    _sources_in
    "Public/*.in"
    "Private/*.in"
)

FOREACH(_file ${_sources_in})
    STRING(REPLACE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        _file_out
        ${_file}
    )

    string(REGEX MATCH "^(.*)\\.[^.]*$" _file_out ${_file_out})
    set(_file_out ${CMAKE_MATCH_1})

    CONFIGURE_FILE(${_file} ${_file_out})
    LIST(APPEND _sources_out ${_file_out})
ENDFOREACH()

SET(_target GBx)

ADD_LIBRARY(
    ${_target}
    ${_sources}
    ${_sources_in}
    ${_sources_out}
)

SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_sources}")
SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_sources_in}")
SET_SOURCE_GROUPS(${CMAKE_CURRENT_BINARY_DIR} "${_sources_out}")

SET_TARGET_PROPERTIES(
    ${_target}
    PROPERTIES
        C_STANDARD 11
)

TARGET_LINK_LIBRARIES(
    ${_target}
    PUBLIC
        clog::clog
)

TARGET_INCLUDE_DIRECTORIES(
    ${_target}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Public>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/Public>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Private>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/Private>
)

TARGET_COMPILE_DEFINITIONS(
    ${_target}
    PRIVATE
        CLOG_SOURCE_PATH="${CMAKE_SOURCE_DIR}/"
)