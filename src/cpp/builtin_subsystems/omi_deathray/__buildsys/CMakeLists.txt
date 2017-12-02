set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/build/linux_x86/subsystems
)

include_directories(
    ..
)

set(OMI_DEATHRAY_SRC
    ../DeathGlobals.cpp
    ../DeathSubsystem.cpp
    ../renderable/DeathCamera.cpp
    ../renderable/DeathMesh.cpp

    ../deathray/Geometry.cpp
    ../deathray/Renderer.cpp
    ../deathray/TraceOctant.cpp

    ../deathray/impl/Globals.cpp
    ../deathray/impl/Camera.cpp
    ../deathray/impl/Geometric.cpp
    ../deathray/impl/Octant.cpp
    ../deathray/impl/Octree.cpp
    ../deathray/impl/PipelineController.cpp
    ../deathray/impl/Scene.cpp
    ../deathray/impl/Spatial.cpp
    ../deathray/impl/VBO.cpp
    ../deathray/impl/acceleration/GPUOctree.cpp
    ../deathray/impl/debug/GLCells.cpp
    ../deathray/impl/debug/GLGeometry.cpp
    ../deathray/impl/debug/GLOctree.cpp
    ../deathray/impl/pathtracer/cpu/CPUIntersections.cpp
    ../deathray/impl/pathtracer/cpu/CPULightPath.cpp
    ../deathray/impl/pathtracer/cpu/CPURayHierarchy.cpp
    ../deathray/impl/prefab/PrefabricationPipeline.cpp
    ../deathray/impl/render/BoundRenderer.cpp
    ../deathray/impl/render/CPUPathTracer.cpp
    ../deathray/impl/render/CellRenderer.cpp
    ../deathray/impl/render/GPUPathTracer.cpp
    ../deathray/impl/render/GeometryRenderer.cpp
    ../deathray/impl/render/OctreeRenderer.cpp
    ../deathray/impl/render/OrientationRenderer.cpp
    ../deathray/impl/render/RayRenderer.cpp
    ../deathray/impl/render/RenderPipeline.cpp
    ../deathray/impl/render/View.cpp
    ../deathray/impl/util/DataUtil.cpp

    ../deathray/gl/ErrorState.cpp
    ../deathray/gl/Framebuffer.cpp
    ../deathray/gl/ShaderProgram.cpp
    ../deathray/gl/Texture2D.cpp
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
    m
    IL
    omicron_api
    arcanecore_collate
    arcanecore_config
    arcanecore_log
    arcanecore_thread
    arcanecore_io
    arcanecore_base
)
