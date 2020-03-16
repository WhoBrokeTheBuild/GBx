
# libGBx

FILE(
    GLOB_RECURSE
    _SOURCES
    src/*.h
    src/*.c
)

ADD_LIBRARY(
    libGBx
    ${_SOURCES}
)

SET_TARGET_PROPERTIES(
    libGBx PROPERTIES
        OUTPUT_NAME libGBx
        C_STANDARD 11
)

TARGET_INCLUDE_DIRECTORIES(
    libGBx
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/GBx>
        $<INSTALL_INTERFACE:include>
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

        ADD_TEST(
            NAME ${test_target}
            COMMAND $<TARGET_FILE:${test_target}>
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests"
        )
    ENDFOREACH()
ENDIF()
