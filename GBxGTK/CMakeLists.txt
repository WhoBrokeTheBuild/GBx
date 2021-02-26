
INCLUDE(SetSourceGroups)

FILE(
    GLOB_RECURSE
    _sources
    "Private/*.h"
    "Private/*.c"
)

FILE(
    GLOB_RECURSE
    _resources
    "Resources/*.glade"
    "Resources/*.xml"
)

SET(_resources_dir          ${CMAKE_CURRENT_SOURCE_DIR}/Resources)
SET(_gresource              ${_resources_dir}/gbx.gresource.xml)
SET(_resource_header_out    ${CMAKE_CURRENT_BINARY_DIR}/Private/Resource.h)
SET(_resource_source_out    ${CMAKE_CURRENT_BINARY_DIR}/Private/Resource.c)

FILE(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/Private)

ADD_CUSTOM_COMMAND(
    COMMAND ${GLIB_COMPILE_RESOURCES_PROGRAM} ${_gresource} --target ${_resource_header_out} --generate-header
    COMMAND ${GLIB_COMPILE_RESOURCES_PROGRAM} ${_gresource} --target ${_resource_source_out} --generate-source
    OUTPUT ${_resource_header_out} ${_resource_source_out}
    DEPENDS ${_resources}
    WORKING_DIRECTORY ${_resources_dir}
)

SET(_target GBxGTK)

ADD_EXECUTABLE(
    ${_target}
    ${_sources}
    ${_resources}
    ${_resource_header_out}
    ${_resource_source_out}
)

SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_sources}")
SET_SOURCE_GROUPS(${CMAKE_CURRENT_SOURCE_DIR} "${_resources}")
SET_SOURCE_GROUPS(${CMAKE_CURRENT_BINARY_DIR} "${_resource_header_out}")
SET_SOURCE_GROUPS(${CMAKE_CURRENT_BINARY_DIR} "${_resource_source_out}")

SET_TARGET_PROPERTIES(
    ${_target}
    PROPERTIES
        C_STANDARD 11
)

TARGET_LINK_LIBRARIES(
    ${_target}
    PRIVATE
        GBx
        GTK3::GTK3
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