#include <omicron/api/API.hpp>

#include "omi_glfw/GLFWSubsystem.hpp"
#include "omi_glfw/GLFWSurface.hpp"


OMI_CONTEXT_REGISTER_SUBSYSTEM(
    "0.0.1",
    omi_glfw::GLFWSubsystem,
    omi_glfw::GLFWSurface
);
