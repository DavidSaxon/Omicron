set(CONFIG_SRC
    ../ConfigGlobals.cpp
)

add_library(omicron_config SHARED ${CONFIG_SRC})

# link libraries to the omicron runtime
target_link_libraries(omicron_config
    arcanecore_collate
    arcanecore_config
    arcanecore_log
    arcanecore_io
    arcanecore_base
)
