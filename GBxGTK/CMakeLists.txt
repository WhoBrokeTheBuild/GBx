
FILE(
    GLOB_RECURSE
    _resources
    "Resources/*.ui"
)

SET(_resources_dir  ${CMAKE_CURRENT_SOURCE_DIR}/Resources)
SET(_gresource_in   ${_resources_dir}/gbx.gresource.xml)
SET(_gresource_out  ${CMAKE_CURRENT_SOURCE_DIR}/gbx.gresource)

ADD_CUSTOM_COMMAND(
    COMMAND ${GLIB_COMPILE_RESOURCES_PROGRAM} ${_gresource_in} --target=${_gresource_out}
    OUTPUT ${_gresource_out}
    DEPENDS ${_resources} ${_gresource_in}
    WORKING_DIRECTORY ${_resources_dir}
)

ADD_CUSTOM_TARGET(
    GBxGTK ALL
    DEPENDS ${_gresource_out}
)

ADD_CUSTOM_TARGET(
    run-GBxGTK
    COMMAND ${CMAKE_COMMAND} -E env LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/GBx ${Python3_EXECUTABLE} -B -m GBxGTK
    DEPENDS GBxGTK
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)