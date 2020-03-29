
FILE(
    GLOB_RECURSE
    private
    "private/*.h"
    "private/*.c"
)

FILE(
    GLOB_RECURSE
    public
    "public/*.h"
)

ADD_LIBRARY(GBx "")

TARGET_SOURCES(
    GBx
    PRIVATE
        ${private}
        ${public}
)

SET_TARGET_PROPERTIES(
    GBx PROPERTIES
        C_STANDARD 11
)

TARGET_INCLUDE_DIRECTORIES(
    GBx
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/private>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
        $<INSTALL_INTERFACE:include>
)

IF(Readline_FOUND)
    TARGET_LINK_LIBRARIES(
        GBx
        PUBLIC
            Readline::Readline
    )
    
    TARGET_COMPILE_DEFINITIONS(
        GBx
        PUBLIC
            -DHAVE_READLINE
    )
ENDIF()

# Tests

IF(BUILD_TESTS)
    FILE(GLOB_RECURSE
        tests
        "tests/*_test.c"
    )

    FOREACH(test_source ${tests})
        GET_FILENAME_COMPONENT(test_target ${test_source} NAME_WE)
        
        ADD_EXECUTABLE(
            ${test_target}
            ${test_source}
        )

        TARGET_LINK_LIBRARIES(
            ${test_target}
            PRIVATE
                GBx
        )

        ADD_TEST(
            NAME ${test_target}
            COMMAND $<TARGET_FILE:${test_target}>
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests"
        )
    ENDFOREACH()
ENDIF()
