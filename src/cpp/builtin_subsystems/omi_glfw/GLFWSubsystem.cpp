#include "omi_glfw/GLFWSubsystem.hpp"

#include <omicron/api/report/Logging.hpp>

#include <GLFW/glfw3.h>

#include "omi_glfw/GLFWGlobals.hpp"
#include "omi_glfw/GLFWSurface.hpp"


namespace omi_glfw
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

GLFWSubsystem::GLFWSubsystem()
    : omi::context::ContextSubsystem()
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

GLFWSubsystem::~GLFWSubsystem()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

bool GLFWSubsystem::startup_routine()
{
    // set up logging
    global::logger =
        omi::report::log_handler.vend_input(arc::log::Profile("OMICRON-GLFW"));

    global::logger->debug
        << "Starting Omicron GLFW context subsystem." << std::endl;

    if(!glfwInit())
    {
        // TODO: report
        return false;
    }

    //open the window
    GLFWSurface* surface =
        static_cast<GLFWSurface*>(omi::context::Surface::instance());
    if(!surface->open())
    {
        return false;
    }

    return true;
}

bool GLFWSubsystem::shutdown_routine()
{
    global::logger->debug
        << "Shutting down Omicron GLFW context subsystem." << std::endl;

    glfwTerminate();

    // remove the logger (NOTE: this shouldn't need to be done, but on Windows:
    // closing this DLL causes the memory for the logging input to be freed,
    // even though this DLL doesn't own it).
    omi::report::log_handler.remove_input(global::logger);

    return true;
}

void GLFWSubsystem::main_loop(EngineCycleFunc* engine_cycle_func)
{
    GLFWSurface* surface =
        static_cast<GLFWSurface*>(omi::context::Surface::instance());

    while (!surface->should_close())
    {
        engine_cycle_func();

        surface->swap_buffers();

        // TODO:
        glfwPollEvents();
    }
}

} // namespace omi_glfw
