set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/build/linux_x86/subsystems
)

# source files for the omicron runtime
set(OMI_AL_SRC
    ../ALSubsystem.cpp
)

# build the omicron runtime executable
add_library(omi_al SHARED ${OMI_AL_SRC})

# don't prepend with lib
set_target_properties(omi_al
    PROPERTIES
    PREFIX
    ""
)
# link libraries
target_link_libraries(omi_al
    omicron_report
    arcanecollate_lib
    metaengine
    arcanelog
    arcanecore_io
    arcanecore_base
)
