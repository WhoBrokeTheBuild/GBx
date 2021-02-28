
INCLUDE(SetSourceGroups)

FILE(
    GLOB_RECURSE
    _sources
    "Private/*.h"
    "Private/*.c"
)

SET(_target GBxASM)

ADD_EXECUTABLE(
    ${_target}
    ${_sources}
)

SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_sources}")

SET_TARGET_PROPERTIES(
    ${_target}
    PROPERTIES
        C_STANDARD 11
)

TARGET_LINK_LIBRARIES(
    ${_target}
    PRIVATE
        cflags::cflags
)

TARGET_INCLUDE_DIRECTORIES(
    ${_target}
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Private>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/Private>
)

TARGET_COMPILE_DEFINITIONS(
    ${_target}
    PRIVATE
        CLOG_SOURCE_PATH="${CMAKE_SOURCE_DIR}/"
)