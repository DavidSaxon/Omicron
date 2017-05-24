set(API_SRC
    ../config/ConfigGlobals.cpp

    ../report/Logging.cpp
    ../report/ReportBoot.cpp
    ../report/ReportGlobals.cpp
)

add_library(omicron_api SHARED ${API_SRC})

target_link_libraries(omicron_api
    arcanecore_collate
    arcanecore_config
    arcanecore_log
    arcanecore_io
    arcanecore_base
)
