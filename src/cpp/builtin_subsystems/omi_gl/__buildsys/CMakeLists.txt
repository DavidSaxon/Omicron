set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/build/linux_x86/subsystems
)

# source files for the omicron runtime
set(OMI_GL_SRC
    ../GLBootstrap.cpp
    ../GLGlobals.cpp
)

# build the omicron runtime executable
add_library(omi_gl SHARED ${OMI_GL_SRC})

# don't prepend with lib
set_target_properties(omi_gl
    PROPERTIES
    PREFIX
    ""
)
# link libraries
target_link_libraries(omi_gl
    ${GLEW_LIBRARIES}
    omicron_api
    arcanecore_collate
    arcanecore_config
    arcanecore_log
    arcanecore_io
    arcanecore_base
)
