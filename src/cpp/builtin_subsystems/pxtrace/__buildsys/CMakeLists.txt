set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/build/linux_x86/subsystems
)

# source files for the omicron runtime
set(PXTRACE_SRC
    ../Camera.cpp
    ../FrameBuffer.cpp
    ../PXGlobals.cpp
    ../PXSubsystem.cpp
    ../Sphere.cpp
)

# build the omicron runtime executable
add_library(pxtrace SHARED ${PXTRACE_SRC})

# don't prepend with lib
set_target_properties(pxtrace
    PROPERTIES
    PREFIX
    ""
)
# link libraries
target_link_libraries(pxtrace
    ${GLEW_LIBRARIES}
    omicron_api
    arcanecore_collate
    arcanecore_config
    arcanecore_log
    arcanecore_io
    arcanecore_base
)
