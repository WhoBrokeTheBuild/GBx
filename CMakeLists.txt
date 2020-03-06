CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

# Allow for custom CMake modules
LIST(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake)

###
### Project
###

PROJECT(
    GBx 
    LANGUAGES C 
    VERSION 0.1.0
)

###
### Options
###

OPTION(BUILD_TOOLS      "Build Tools"                               OFF)
OPTION(BUILD_TESTS      "Build Tests"                               OFF)
OPTION(BUILD_THIRDPARTY "Build Missing Third-Party Dependencies"    ON)

###
### Configuration
###

# Allow for custom organization of files in VisualStudio
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

IF(BUILD_TESTS)
    ENABLE_TESTING()
ENDIF()

###
### Third Party Dependencies
###

IF(BUILD_THIRDPARTY)
    SET(THIRDPARTY_SOURCE_DIR ${CMAKE_SOURCE_DIR}/thirdparty)
    SET(THIRDPARTY_BINARY_DIR ${CMAKE_BINARY_DIR}/thirdparty)

    FILE(MAKE_DIRECTORY ${THIRDPARTY_BINARY_DIR})

    # Set _ROOT variables for config-based packages
    SET(cflags_ROOT ${THIRDPARTY_BINARY_DIR}/cflags)
    SET(SDL2_ROOT   ${THIRDPARTY_BINARY_DIR}/SDL2)

    EXECUTE_PROCESS(
        COMMAND ${CMAKE_COMMAND} 
            -G "${CMAKE_GENERATOR}" 
            -A "${CMAKE_GENERATOR_PLATFORM}"
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
            ${THIRDPARTY_SOURCE_DIR}
        WORKING_DIRECTORY ${THIRDPARTY_BINARY_DIR}
        RESULT_VARIABLE _result
    )
    IF(_result)
        MESSAGE("${_result}")
        MESSAGE(FATAL_ERROR "Failed to configure THIRDPARTY projects")
    ENDIF()

    SET(_THIRDPARTY_BUILD_COMMAND ${CMAKE_COMMAND} --build . )
    IF(CMAKE_GENERATOR STREQUAL "Unix Makefiles")
        SET(_THIRDPARTY_BUILD_COMMAND ${CMAKE_MAKE_PROGRAM} -l )
    ENDIF()

    EXECUTE_PROCESS(
        COMMAND ${_THIRDPARTY_BUILD_COMMAND}
        WORKING_DIRECTORY ${THIRDPARTY_BINARY_DIR}
        RESULT_VARIABLE _result
    )

    IF(_result)
        MESSAGE("${_result}")
        MESSAGE(FATAL_ERROR "Failed to build THIRDPARTY projects")
    ENDIF()
ENDIF()

FIND_PACKAGE(SDL2 2.0.6 CONFIG REQUIRED)
FIND_PACKAGE(cflags CONFIG REQUIRED)
FIND_PACKAGE(Readline)

GET_TARGET_PROPERTY(_location SDL2::SDL2 IMPORTED_LOCATION_RELEASE)
GET_FILENAME_COMPONENT(SDL2_RUNTIME_DIR ${_location} DIRECTORY)

SET(RUNTIME_SEARCH_PATH
    ${SDL2_RUNTIME_DIR}
)

# Library

ADD_SUBDIRECTORY(libGBx)

# Executable

ADD_SUBDIRECTORY(GBx)

IF(BUILD_TOOLS)
    ADD_SUBDIRECTORY(tools/romdump)
ENDIF()

# Set VisualStudio startup project
SET_PROPERTY(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "GBx")