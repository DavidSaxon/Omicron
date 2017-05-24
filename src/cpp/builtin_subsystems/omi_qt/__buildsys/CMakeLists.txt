set(CMAKE_AUTOMOC ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_BINARY_DIR}/build/linux_x86/subsystems
)

# source files for the omicron runtime
set(OMI_QT_SRC
    ../MainWindow.cpp
    ../QTGlobals.cpp
    ../QTSubsystem.cpp
)

# build the omicron runtime executable
add_library(omi_qt SHARED ${OMI_QT_SRC})

# don't prepend with lib
set_target_properties(omi_qt
    PROPERTIES
    PREFIX
    ""
)
# link libraries
target_link_libraries(omi_qt
    Qt5::Widgets
    Qt5::Gui
    Qt5::Core
    omicron_api
    arcanecore_collate
    arcanecore_config
    arcanecore_log
    arcanecore_io
    arcanecore_base
)

set(CMAKE_AUTOMOC OFF)
