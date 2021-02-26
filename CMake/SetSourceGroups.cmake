
MACRO(SET_SOURCE_GROUPS _base_path _files)
    FOREACH(_file IN ITEMS ${_files})
        FILE(RELATIVE_PATH _file_path ${_base_path} ${_file})
        GET_FILENAME_COMPONENT(_file_path ${_file_path} DIRECTORY)
        FILE(TO_NATIVE_PATH ${_file_path} _file_path)
        SOURCE_GROUP(${_file_path} FILES ${_file})
    ENDFOREACH()
ENDMACRO()