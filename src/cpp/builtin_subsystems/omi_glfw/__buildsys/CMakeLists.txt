set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/build/linux_x86/subsystems
)

include_directories(
    ../../../../../ext/glfw-3.2.1/include
)

link_directories(
    ../../../../../ext/glfw-3.2.1/lib/linux_x86
)

set(OMI_GFLW_SRC
    ../GLFWGlobals.cpp
    ../GLFWSubsystem.cpp
    ../GLFWSurface.cpp
    ../OmiGLFWRegister.cpp
)

# build the omicron runtime executable
add_library(omi_glfw SHARED ${OMI_GFLW_SRC})

# don't prepend with lib
set_target_properties(omi_glfw
    PROPERTIES
    PREFIX
    ""
)
# link libraries
target_link_libraries(omi_glfw
    glfw3
    X11
    Xi
    Xrandr
    Xxf86vm
    Xinerama
    Xcursor
    rt
    m
    pthread
    omicron_api
    arcanecore_collate
    arcanecore_config
    arcanecore_log
    arcanecore_io
    arcanecore_base
)
