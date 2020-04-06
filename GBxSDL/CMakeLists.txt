
# GBx

FILE(
    GLOB_RECURSE
    private
    "private/*.h"
    "private/*.c"
)

ADD_EXECUTABLE(
    GBxSDL
    ${private}
)

TARGET_LINK_LIBRARIES(
    GBxSDL
    PUBLIC
        GBx
        SDL2::SDL2
        DUI::DUI
        cflags::cflags
)

TARGET_COMPILE_DEFINITIONS(
    GBxSDL
    PRIVATE
        # -DDUI_FONT_GB
        -DDUI_FONT_ANONYMOUS
)

SET_TARGET_PROPERTIES(
    GBxSDL PROPERTIES
        C_STANDARD 11
)

IF(TARGET SDL2::SDL2main)
    TARGET_LINK_LIBRARIES(
        GBxSDL
        PUBLIC
            SDL2::SDL2main
    )
ENDIF()
