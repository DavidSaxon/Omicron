set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/build/linux_x86
)

set(HELLBOUND_SRC
    ../GameRegistration.cpp
    ../HellboundGlobals.cpp
    ../entities/GameManager.cpp
    ../entities/prototype/Enemy.cpp
    ../entities/prototype/ProtoEntity.cpp
    ../scripts/Startup.cpp
)

# build the omicron runtime executable
add_library(hellbound SHARED ${HELLBOUND_SRC})

# don't prepend with lib
set_target_properties(hellbound
    PROPERTIES
    PREFIX
    ""
)
# link libraries
target_link_libraries(hellbound
    omicron_api
    arcanecore_collate
    arcanecore_config
    arcanecore_log
    arcanecore_io
    arcanecore_base
)
