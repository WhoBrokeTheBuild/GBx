
# libGBx

FILE(
    GLOB_RECURSE
    _SOURCES
    src/*.h
    src/*.c
)

LIST(
    REMOVE_ITEM 
    _SOURCES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
)

ADD_LIBRARY(
    libGBx
    ${_SOURCES}
)

TARGET_INCLUDE_DIRECTORIES(
    libGBx
    PUBLIC
        src
)

TARGET_LINK_LIBRARIES(
    libGBx
    PUBLIC
        SDL2::SDL2
        cflags::cflags
)

IF(Readline_FOUND)
    TARGET_LINK_LIBRARIES(
        libGBx
        PUBLIC
            Readline::Readline
    )
    TARGET_COMPILE_DEFINITIONS(
        libGBx
        PUBLIC
            -DHAVE_READLINE
    )
ENDIF()

# GBx

ADD_EXECUTABLE(
    GBx
    src/main.c
)

TARGET_LINK_LIBRARIES(
    GBx
    PUBLIC
        libGBx
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

# Tests

IF(BUILD_TESTS)
    FILE(GLOB_RECURSE
        _TESTS
        "tests/*_test.c"
    )

    FOREACH(test_source ${_TESTS})
        GET_FILENAME_COMPONENT(test_target ${test_source} NAME_WE)
        
        ADD_EXECUTABLE(
            ${test_target}
            ${test_source}
        )

        TARGET_LINK_LIBRARIES(
            ${test_target}
            PRIVATE
                libGBx
        )

        IF(TARGET SDL2::SDL2main)
            TARGET_LINK_LIBRARIES(
                ${test_target}
                PUBLIC
                    SDL2::SDL2main
            )
        ENDIF()

        ADD_TEST(
            NAME ${test_target}
            COMMAND $<TARGET_FILE:${test_target}>
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests"
        )
    ENDFOREACH()
ENDIF()
