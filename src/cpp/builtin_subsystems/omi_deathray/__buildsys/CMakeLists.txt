set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/build/linux_x86/subsystems
)

include_directories(
    ..
)

set(OMI_DEATHRAY_SRC
    ../DeathGlobals.cpp
    ../DeathSubsystem.cpp
    ../renderable/DeathMesh.cpp

    ../deathray/Geometry.cpp
    ../deathray/Renderer.cpp
    ../deathray/TraceOctant.cpp
    ../deathray/gl/ErrorState.cpp
    ../deathray/gl/ShaderProgram.cpp
)

# build the omicron runtime executable
add_library(omi_deathray SHARED ${OMI_DEATHRAY_SRC})

# don't prepend with lib
set_target_properties(omi_deathray
    PROPERTIES
    PREFIX
    ""
)
# link libraries
target_link_libraries(omi_deathray
    ${GLEW_LIBRARIES}
    omicron_api
    arcanecore_collate
    arcanecore_config
    arcanecore_log
    arcanecore_io
    arcanecore_base
)
