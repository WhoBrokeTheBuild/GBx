
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

SET(target SM83)

ADD_LIBRARY(${target} "")

TARGET_SOURCES(
    ${target}
    PRIVATE
        ${private}
        ${public}
)

SET_TARGET_PROPERTIES(
    ${target} PROPERTIES
        C_STANDARD 11
)

TARGET_INCLUDE_DIRECTORIES(
    ${target}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/private>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>
        $<INSTALL_INTERFACE:include>
)

IF(ClangFormat_FOUND)
    ADD_CUSTOM_TARGET(
        format-${target}
        COMMAND ${ClangFormat_PROGRAM} -i ${public} ${private}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )

    SET_TARGET_PROPERTIES(
        format-${target}
        PROPERTIES 
            FOLDER "Automation"
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
                ${target}
        )

        ADD_TEST(
            NAME ${test_target}
            COMMAND $<TARGET_FILE:${test_target}>
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests"
        )
    ENDFOREACH()
ENDIF()
