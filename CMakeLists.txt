cmake_minimum_required(VERSION 3.2)
project(Omicron)

set(PROTO_BULLET_HELLOWORLD_SRC
    proto/bullet/HelloWorld.cpp
)

set(OMICRON_SRC
    src/cpp/omicron/Main.cpp
    src/cpp/omicron/Engine.cpp
    src/cpp/omicron/private/Logging.cpp
    src/cpp/omicron/private/SubsystemManager.cpp
    src/cpp/omicron/private/boot/BootRoutines.cpp
    src/cpp/omicron/private/util/MetaUtil.cpp
    src/cpp/omicron/runtime/clock/AbstractClock.cpp
    src/cpp/omicron/runtime/clock/GameClock.cpp
    src/cpp/omicron/runtime/clock/WallClock.cpp



    src/cpp/omicron/proto/Entity.cpp
    src/cpp/omicron/proto/Scene.cpp

    src/cpp/omicron/TestEntity.cpp
    src/cpp/omicron/TestScene.cpp
)

set(QTO_SRC
    src/cpp/subsystems/qto/MainWindow.cpp
    src/cpp/subsystems/qto/QTOSubsystemRegister.cpp
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)

# TODO: sort out flags (especially -g, and build for different optimisation
#      levels)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++0x -Wall -Wno-varargs -fPIC")

include_directories(
    ${INCLUDE_DIRECTORIES}
    src/cpp
    # TODO: eventually replace with proper install paths
    ${CMAKE_BINARY_DIR}/../../ArcaneCore/ArcaneCore/src/cpp
    ${CMAKE_BINARY_DIR}/../../ArcaneLog/ArcaneLog/src/cpp
    ${CMAKE_BINARY_DIR}/../../MetaEngine/MetaEngine/src/cpp
    ext/bullet/include
    ext/bullet/include/bullet
)

link_directories(
    ${LINK_DIRECTORIES}
    # TODO: eventually replace with proper install paths
    ${CMAKE_BINARY_DIR}/../../ArcaneCore/ArcaneCore/build/linux_x86
    ${CMAKE_BINARY_DIR}/../../ArcaneLog/ArcaneLog/build/linux_x86
    ${CMAKE_BINARY_DIR}/../../MetaEngine/MetaEngine/build/linux_x86
    ext/bullet/lib/linux_x86
)

#add_executable(proto_bullet_helloworld ${PROTO_BULLET_HELLOWORLD_SRC})

#target_link_libraries(proto_bullet_helloworld
#    BulletDynamics
#    BulletCollision
#    LinearMath
#)

add_executable(omicron ${OMICRON_SRC})
target_link_libraries(omicron
    metaengine
    arcanelog_shared
    arcanelog
    arcanecore_io
    arcanecore_base
    dl
)

# set subsystem output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86/subsystems)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86/subsystems)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86/subsystems)

set(CMAKE_AUTOMOC ON)

# TODO:
#set(AUTOGEN_TARGETS_FOLDER ${CMAKE_BINARY_DIR}/gen/cpp)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

add_library(qto SHARED ${QTO_SRC})
SET_TARGET_PROPERTIES(qto PROPERTIES PREFIX "")
target_link_libraries(qto
    metaengine
    arcanelog_shared
    arcanelog
    arcanecore_io
    arcanecore_base
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

set(CMAKE_AUTOMOC OFF)
