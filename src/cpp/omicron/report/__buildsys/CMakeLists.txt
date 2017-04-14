set(REPORT_SRC
    ../Logging.cpp
    ../ReportBoot.cpp
    ../ReportGlobals.cpp
)

add_library(omicron_report SHARED ${REPORT_SRC})

# link libraries to the omicron runtime
target_link_libraries(omicron_report
    omicron_meta
    arcanecollate_lib
    metaengine
    arcanelog
    arcanecore_io
    arcanecore_base
)
