CMAKE_MINIMUM_REQUIRED(VERSION 3.15)

PROJECT(thirdparty)

INCLUDE(ExternalProject)

# cflags

SET(cflags_ROOT       ${CMAKE_BINARY_DIR}/cflags)
SET(cflags_SOURCE_DIR ${CMAKE_BINARY_DIR}/cflags_src)

FIND_PACKAGE(cflags CONFIG QUIET)

IF(NOT cflags_FOUND)
    ExternalProject_Add(
        thirdparty_cflags
        GIT_REPOSITORY      "https://github.com/WhobrokeTheBuild/cflags.git"
        GIT_TAG             "master"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          ${cflags_SOURCE_DIR}
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${cflags_ROOT}
        BUILD_COMMAND       ""
        UPDATE_COMMAND      ""
    )
ENDIF()

# SDL2

SET(SDL2_ROOT       ${CMAKE_BINARY_DIR}/SDL2)
SET(SDL2_SOURCE_DIR ${CMAKE_BINARY_DIR}/SDL2_src)
SET(SDL2_BINARY_DIR ${CMAKE_BINARY_DIR}/SDL2_bin)

FIND_PACKAGE(SDL2 2.0.6 CONFIG QUIET)

IF(NOT SDL2_FOUND)
    IF(MSVC) 
        SET(SDL2_PATCH_COMMAND ${GIT_EXECUTABLE} apply --ignore-space-change --ignore-whitespace -v ${CMAKE_SOURCE_DIR}/SDL2-vcruntime.patch || ver > nul)
    ENDIF()

    ExternalProject_Add(
        thirdparty_SDL2
        GIT_REPOSITORY      "https://github.com/spurious/SDL-mirror.git"
        GIT_TAG             "master"
        GIT_SHALLOW         TRUE
        SOURCE_DIR          ${SDL2_SOURCE_DIR}
        BINARY_DIR          ${SDL2_BINARY_DIR}
        CMAKE_ARGS          -DCMAKE_INSTALL_PREFIX=${SDL2_ROOT}
                            -DBUILD_SHARED_LIBS=ON
        BUILD_COMMAND       ""
        UPDATE_COMMAND      ""
        TEST_COMMAND        ""
        INSTALL_COMMAND     ${CMAKE_COMMAND} --build . --target install --config Release
        PATCH_COMMAND       ${SDL2_PATCH_COMMAND}
    )
ENDIF()
