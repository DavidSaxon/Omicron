set(META_SRC
    ../MetaGlobals.cpp
)

add_library(omicron_meta SHARED ${META_SRC})

# link libraries to the omicron runtime
target_link_libraries(omicron_meta
    arcanecollate_lib
    metaengine
    arcanelog
    arcanecore_io
    arcanecore_base
)
