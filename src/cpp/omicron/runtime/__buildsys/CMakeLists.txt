# source files for the omicron runtime
set(OMICRON_RUNTIME_SRC
    ../Main.cpp
    ../Engine.cpp
    ../base/Logging.cpp
    ../base/SubsystemManager.cpp
    ../boot/BootRoutines.cpp
    ../meta/MetaGlobals.cpp
)

# build the omicron runtime executable
add_executable(omicron ${OMICRON_RUNTIME_SRC})

# link libraries to the omicron runtime
target_link_libraries(omicron
    arcanecollate_lib
    metaengine
    arcanelog_shared
    arcanelog
    arcanecore_io
    arcanecore_base
    dl
)
