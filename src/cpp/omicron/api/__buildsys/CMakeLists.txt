set(API_SRC
    ../common/attribute/ArrayAttribute.cpp
    ../common/attribute/Attribute.cpp
    ../common/attribute/BoolAttribute.cpp
    ../common/attribute/ByteAttribute.cpp
    ../common/attribute/DataAttribute.cpp
    ../common/attribute/DoubleAttribute.cpp
    ../common/attribute/FloatAttribute.cpp
    ../common/attribute/Int16Attribute.cpp
    ../common/attribute/Int32Attribute.cpp
    ../common/attribute/Int64Attribute.cpp
    ../common/attribute/MapAttribute.cpp
    ../common/attribute/PathAttribute.cpp
    ../common/attribute/StringAttribute.cpp

    ../config/ConfigGlobals.cpp

    ../asset/AssetGlobals.cpp
    ../asset/AssetLibrary.cpp
    ../asset/loaders/OBJLoader.cpp
    ../asset/types/Geometry.cpp

    ../report/Logging.cpp
    ../report/ReportBoot.cpp
    ../report/ReportGlobals.cpp

    ../res/ResourceGlobals.cpp
    ../res/ResourceId.cpp
    ../res/ResourceRegistry.cpp

    ../window/MainWindow.cpp
)

add_library(omicron_api SHARED ${API_SRC})

target_link_libraries(omicron_api
    arcanecore_collate
    arcanecore_config
    arcanecore_log
    arcanecore_crypt
    arcanecore_io
    arcanecore_base
)
