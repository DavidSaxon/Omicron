# source files for the omicron runtime
set(OMICRON_RUNTIME_SRC
    ../Main.cpp
    ../Engine.cpp
    ../RuntimeGlobals.cpp
    ../SubsystemManager.cpp
    ../boot/BootLogging.cpp
    ../boot/BootRoutines.cpp
)

# build the omicron runtime executable
add_executable(omicron ${OMICRON_RUNTIME_SRC})

# link libraries to the omicron runtime
target_link_libraries(omicron
    omicron_report
    omicron_meta
    arcanecollate_lib
    metaengine
    arcanelog
    arcanecore_io
    arcanecore_base
    dl
)
