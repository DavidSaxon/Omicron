set(REPORT_SRC
    ../Logging.cpp
    ../ReportBoot.cpp
    ../ReportGlobals.cpp
)

add_library(omicron_report SHARED ${REPORT_SRC})

# link libraries to the omicron runtime
target_link_libraries(omicron_report
    omicron_config
    arcanecore_collate
    arcanecore_config
    arcanecore_json
    arcanecore_log
    arcanecore_io
    arcanecore_base
)
