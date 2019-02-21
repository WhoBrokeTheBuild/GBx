
FIND_PATH(
    Readline_INCLUDE_DIRS
    NAMES 
        readline/readline.h
    HINTS 
        ${Readline_ROOT_DIR}
    PATH_SUFFIXES
        include/
)

FIND_LIBRARY(Readline_LIBRARIES
    NAMES 
        readline
    HINTS 
        ${Readline_ROOT_DIR}
    PATH_SUFFIXES
        lib
)

INCLUDE(FindPackageHandleStandardArgs)

IF(Readline_INCLUDE_DIRS AND Readline_LIBRARIES)
    SET(Readline_FOUND TRUE)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(
        Readline 
        DEFAULT_MSG 
        Readline_INCLUDE_DIRS
        Readline_LIBRARIES
    )
    ADD_LIBRARY(Readline::Readline INTERFACE IMPORTED)
    SET_TARGET_PROPERTIES(Readline::Readline PROPERTIES 
        INTERFACE_INCLUDE_DIRECTORIES ${Readline_INCLUDE_DIRS}
        INTERFACE_LINK_LIBRARIES ${Readline_LIBRARIES}
    )
ENDIF()

MARK_AS_ADVANCED(
    Readline_INCLUDE_DIR
    Readline_LIBRARY
)