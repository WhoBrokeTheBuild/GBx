
# GBx

FILE(
    GLOB_RECURSE
    _SOURCES
    src/*.h
    src/*.c
)

ADD_EXECUTABLE(
    GBx
    ${_SOURCES}
)

TARGET_LINK_LIBRARIES(
    GBx
    PUBLIC
        libGBx
        SDL2::SDL2
        cflags::cflags
)

IF(MSVC)
    SET_TARGET_PROPERTIES(
        GBx
        PROPERTIES 
            VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${RUNTIME_SEARCH_PATH}"
    )
ENDIF()

IF(TARGET SDL2::SDL2main)
    TARGET_LINK_LIBRARIES(
        GBx
        PUBLIC
            SDL2::SDL2main
    )
ENDIF()