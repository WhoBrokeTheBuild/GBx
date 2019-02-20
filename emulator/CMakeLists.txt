
ADD_EXECUTABLE(
    GBx
    alu.c
    bootstrap.c
    cpu.c
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
    Threads::Threads
)

IF(VERBOSE)
    TARGET_COMPILE_DEFINITIONS(
        GBx
        PRIVATE
            -DVERBOSE
    )
ENDIF()