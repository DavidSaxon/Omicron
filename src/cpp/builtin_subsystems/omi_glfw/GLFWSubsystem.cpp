#include "omi_glfw/GLFWSubsystem.hpp"

#include <GLFW/glfw3.h>

// TODO: REMOVE ME
#include <iostream>


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
    // TODO: REMOVE ME
    std::cout << "FUCK YEAH!" << std::endl;

    if(!glfwInit())
    {
        // TODO: report
        return false;
    }

    // TODO:
    return true;
}

bool GLFWSubsystem::shutdown_routine()
{
    // TODO:
    return true;
}

} // namespace omi_glfw
