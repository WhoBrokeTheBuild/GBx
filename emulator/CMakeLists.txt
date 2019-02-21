
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
    video.c
)

TARGET_LINK_LIBRARIES(
    GBx
    SDL2::SDL2
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

IF(LOG_VERBOSE)
    TARGET_COMPILE_DEFINITIONS(
        GBx
        PRIVATE
            -DLOG_VERBOSE
    )
ENDIF()

IF(LOG_INST)
    TARGET_COMPILE_DEFINITIONS(
        GBx
        PRIVATE
            -DLOG_INST
    )
ENDIF()