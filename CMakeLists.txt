cmake_minimum_required(VERSION 3.2)
project(Omicron)

# set default build directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build/linux_x86)

# TODO: how to control debug builds - and compiler optimisations
# compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -std=c++0x -Wall -Wno-varargs -fPIC -msse3 -mfpmath=sse")

# require GLEW
find_package(GLEW REQUIRED)
# require QT
find_package(OpenGL REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

# set general libraries include directories
include_directories(
    ${INCLUDE_DIRECTORIES}
    src/cpp
    src/cpp/builtin_subsystems
    /usr/include/python3.5
    ${GLEW_INCLUDE_DIRS}
    # TODO: eventually replace with proper install paths
    ${CMAKE_BINARY_DIR}/../../ArcaneCore/ArcaneCore/src/cpp
)
# set general libraries link directories
link_directories(
    ${LINK_DIRECTORIES}
    build/linux_x86/
    /usr/lib/python3.5/
    ${OpenGL_LIBRARY_DIRS}
    # TODO: eventually replace with proper library paths
    ${CMAKE_BINARY_DIR}/../../ArcaneCore/ArcaneCore/build/linux_x86
)

add_definitions(${OpenGL_DEFINITIONS})

# api
add_subdirectory(src/cpp/omicron/api/__buildsys)
# omicron runtime
add_subdirectory(src/cpp/omicron/runtime/__buildsys)
# tests
add_subdirectory(tests/cpp/__buildsys)
# OpenAL subsystem
add_subdirectory(src/cpp/builtin_subsystems/omi_al/__buildsys)
# Bullet subsystem
add_subdirectory(src/cpp/builtin_subsystems/omi_bullet/__buildsys)
# GLFW subsystem
add_subdirectory(src/cpp/builtin_subsystems/omi_glfw/__buildsys)
# DeathRay subsystem
add_subdirectory(src/cpp/builtin_subsystems/omi_deathray/__buildsys)
# hellbound
add_subdirectory(src/cpp/hellbound/__buildsys)
