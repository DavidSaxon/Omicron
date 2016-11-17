cmake_minimum_required(VERSION 3.2)
project(Omicron)

set(PROTO_BULLET_HELLOWORLD_SRC
    proto/bullet/HelloWorld.cpp
)

set(OMICRON_SRC
    src/omicron/runtime/clock/AbstractClock.cpp
    src/omicron/runtime/clock/GameClock.cpp
    src/omicron/runtime/clock/WallClock.cpp
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)

# TODO: sort out flags (especially -g, and build for different optimisation
#      levels)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++0x -Wall -Wno-varargs -fPIC")

include_directories(
    ${INCLUDE_DIRECTORIES}
    src
    # TODO: eventually replace with proper install paths
    ${CMAKE_BINARY_DIR}/../../ArcaneCore/ArcaneCore/src/cpp
    ext/bullet/include
    ext/bullet/include/bullet
)

link_directories(
    ${LINK_DIRECTORIES}
    # TODO: eventually replace with proper install paths
    ${CMAKE_BINARY_DIR}/../../ArcaneCore/ArcaneCore/build/linux_x86
    ext/bullet/lib/linux_x86
)

#add_executable(proto_bullet_helloworld ${PROTO_BULLET_HELLOWORLD_SRC})

#target_link_libraries(proto_bullet_helloworld
#    BulletDynamics
#    BulletCollision
#    LinearMath
#)

add_library(libomicron ${OMICRON_SRC})

target_link_libraries(libomicron
    arcanecore_io
    arcanecore_base
)
