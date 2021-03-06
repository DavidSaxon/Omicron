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

    ../context/ContextSubsystem.cpp
    ../context/Event.cpp
    ../context/EventListener.cpp
    ../context/EventService.cpp
    ../context/Surface.cpp

    ../render/RenderSubsystem.cpp

    ../report/Logging.cpp
    ../report/ReportBoot.cpp
    ../report/ReportGlobals.cpp
    ../report/SystemMonitor.cpp
    ../report/stats/StatsDatabase.cpp
    ../report/stats/StatsOperations.cpp
    ../report/stats/StatsQuery.cpp

    ../res/ResourceGlobals.cpp
    ../res/ResourceId.cpp
    ../res/ResourceRegistry.cpp
    ../res/loaders/OBJLoader.cpp
    ../res/loaders/RawLoader.cpp

    ../scene/Entity.cpp
    ../scene/SceneGlobals.cpp
    ../scene/SceneState.cpp
    ../scene/component/AbstractComponent.cpp
    ../scene/component/renderable/Camera.cpp
    ../scene/component/renderable/Mesh.cpp
    ../scene/component/transform/AbstractTransform.cpp
    ../scene/component/transform/AxisAngleTransform.cpp
    ../scene/component/transform/MatrixTransform.cpp
    ../scene/component/transform/QuaternionTransform.cpp
    ../scene/component/transform/Scale3Transform.cpp
    ../scene/component/transform/ScaleTransform.cpp
    ../scene/component/transform/TranslateTransform.cpp
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
