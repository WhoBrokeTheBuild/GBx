
ADD_EXECUTABLE(
    GBx
    alu.c
    bootstrap.c
    cpu.c
    debug.c
    inst/CB.c
    interrupt.c
    io.c
    main.c
    memory.c
    register.c
    sound.c
    timer.c
    tinycthread.c
    video.c
)

TARGET_INCLUDE_DIRECTORIES(
    GBx
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

TARGET_LINK_LIBRARIES(
    GBx
    SDL2::SDL2
    Threads::Threads
)

IF(Readline_FOUND)
    TARGET_LINK_LIBRARIES(
        GBx
        Readline::Readline
    )
    TARGET_COMPILE_DEFINITIONS(
        GBx
        PRIVATE
            -DHAVE_READLINE
    )
ENDIF()

IF(VERBOSE)
    TARGET_COMPILE_DEFINITIONS(
        GBx
        PRIVATE
            -DVERBOSE
    )
ENDIF()
