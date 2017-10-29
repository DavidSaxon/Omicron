# source files for the omicron runtime
set(OMICRON_RUNTIME_SRC
    ../Main.cpp
    ../Engine.cpp
    ../RuntimeGlobals.cpp
    ../boot/BootLogging.cpp
    ../boot/BootRoutines.cpp
    ../game/GameBinding.cpp
    ../subsystem/ContextSSDL.cpp
    ../subsystem/RenderSSDL.cpp
    ../subsystem/SubsystemManager.cpp
)

# build the omicron runtime executable
add_executable(omicron ${OMICRON_RUNTIME_SRC})

# link libraries to the omicron runtime
target_link_libraries(omicron
    omicron_api
    arcanecore_collate
    arcanecore_config
    arcanecore_json
    arcanecore_log
    arcanecore_io
    arcanecore_base
    dl
)
