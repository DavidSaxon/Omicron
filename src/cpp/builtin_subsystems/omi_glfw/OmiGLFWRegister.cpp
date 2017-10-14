#include <omicron/api/API.hpp>

#include "omi_glfw/GLFWSubsystem.hpp"
#include "omi_glfw/GLFWSurface.hpp"


extern "C"
{

OMI_API_GLOBAL void* OMI_CONTEXT_subsystem_factory()
{
    return new omi_glfw::GLFWSubsystem();
}

OMI_API_GLOBAL void OMI_CONTEXT_subsystem_destroy(void* self)
{
    delete static_cast<omi_glfw::GLFWSubsystem*>(self);
}

OMI_API_GLOBAL void* OMI_CONTEXT_surface_factory()
{
    return new omi_glfw::GLFWSurface();
}

OMI_API_GLOBAL void OMI_CONTEXT_surface_destroy(void* self)
{
    delete static_cast<omi_glfw::GLFWSurface*>(self);
}

} // extern "C"
