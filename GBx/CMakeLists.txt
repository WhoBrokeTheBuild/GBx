
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
        DUI::DUI
        cflags::cflags
)

TARGET_COMPILE_DEFINITIONS(
    GBx
    PRIVATE
        -DDUI_CHAR_SCALE=2
)

SET_TARGET_PROPERTIES(
    GBx PROPERTIES
        C_STANDARD 11
)

IF(TARGET SDL2::SDL2main)
    TARGET_LINK_LIBRARIES(
        GBx
        PUBLIC
            SDL2::SDL2main
    )
ENDIF()
