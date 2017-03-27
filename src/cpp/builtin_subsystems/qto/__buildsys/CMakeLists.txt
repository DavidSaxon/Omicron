set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/build/linux_x86/subsystems
)

# source files for the omicron runtime
set(QTO_SRC
    ../QTOSubsystem.cpp
)

# build the omicron runtime executable
add_library(qto SHARED ${QTO_SRC})

# don't prepend with lib
set_target_properties(qto
    PROPERTIES
    PREFIX
    ""
)
# link libraries to the omicron runtime
target_link_libraries(qto
    arcanecollate_lib
    metaengine
    arcanelog_shared
    arcanelog
    arcanecore_io
    arcanecore_base
)
