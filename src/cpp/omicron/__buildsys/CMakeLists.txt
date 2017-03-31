# source files for the omicron lib
set(OMICRON_LIB_SRC
    ../Proto.cpp
)

# build the omicron runtime executable
add_library(libomicron SHARED ${OMICRON_LIB_SRC})

# link libraries to the omicron runtime
target_link_libraries(libomicron
    arcanecollate_lib
    metaengine
    arcanelog
    arcanecore_io
    arcanecore_base
)
