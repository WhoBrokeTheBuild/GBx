CMAKE_MINIMUM_REQUIRED(VERSION 3.15 FATAL_ERROR)

# Allow for custom CMake modules
LIST(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/CMake)

###
### Project
###

PROJECT(
    GBx
    LANGUAGES C
)

INCLUDE(Version)

###
### Options
###

OPTION(
    BUILD_TOOLS
    "Build Tools"
    OFF
)

OPTION(
    BUILD_TESTS
    "Build Tests"
    OFF
)

OPTION(
    BUILD_THIRDPARTY
    "Build Missing Third-Party Dependencies"
    ON
)

###
### General Configuration
###

IF(BUILD_TESTS)
    ENABLE_TESTING()
ENDIF()

# Allow for custom organization of files in VisualStudio
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

###
### Third Party Dependencies
###

IF(BUILD_THIRDPARTY)
    SET(_thirdparty_source_dir ${CMAKE_SOURCE_DIR}/ThirdParty)
    SET(_thirdparty_binary_dir ${CMAKE_BINARY_DIR}/ThirdParty)

    FILE(MAKE_DIRECTORY ${_thirdparty_binary_dir})

    # Set _ROOT variables for config-based packages
    SET(SDL2_ROOT   ${_thirdparty_binary_dir}/SDL2)
    SET(cflags_ROOT ${_thirdparty_binary_dir}/cflags)
    SET(clog_ROOT   ${_thirdparty_binary_dir}/clog)

    EXECUTE_PROCESS(
        COMMAND ${CMAKE_COMMAND} 
            -G "${CMAKE_GENERATOR}" 
            -A "${CMAKE_GENERATOR_PLATFORM}"
            -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
            ${_thirdparty_source_dir}
        WORKING_DIRECTORY ${_thirdparty_binary_dir}
        RESULT_VARIABLE _result
    )
    IF(_result)
        MESSAGE("${_result}")
        MESSAGE(FATAL_ERROR "Failed to configure ThirdParty projects")
    ENDIF()

    SET(_thirdparty_build_command ${CMAKE_COMMAND} --build . )
    IF(CMAKE_GENERATOR STREQUAL "Unix Makefiles")
        SET(_thirdparty_build_command ${CMAKE_MAKE_PROGRAM} -l )
    ENDIF()

    EXECUTE_PROCESS(
        COMMAND ${_thirdparty_build_command}
        WORKING_DIRECTORY ${_thirdparty_binary_dir}
        RESULT_VARIABLE _result
    )

    IF(_result)
        MESSAGE("${_result}")
        MESSAGE(FATAL_ERROR "Failed to build ThirdParty projects")
    ENDIF()
ENDIF()

# Libraries

FIND_PACKAGE(cflags CONFIG REQUIRED)
FIND_PACKAGE(clog CONFIG REQUIRED)
FIND_PACKAGE(GTK3 REQUIRED)

# Tools

FIND_PACKAGE(Python3 COMPONENTS Interpreter REQUIRED)

# Targets

ADD_SUBDIRECTORY(GBx)
ADD_SUBDIRECTORY(GBxGTK)
