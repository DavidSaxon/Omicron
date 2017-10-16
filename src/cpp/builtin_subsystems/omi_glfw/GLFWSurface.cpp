#include "omi_glfw/GLFWSurface.hpp"


namespace omi_glfw
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

GLFWSurface::GLFWSurface()
    : omi::context::Surface()
    , m_glfw_window        (nullptr)
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

GLFWSurface::~GLFWSurface()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

bool GLFWSurface::open()
{
    // TODO: where to get arguments from
    m_glfw_window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if(m_glfw_window == nullptr)
    {
        return false;
    }

    // TODO: might need to be exposed
    glfwMakeContextCurrent(m_glfw_window);

    return true;
}

bool GLFWSurface::should_close()
{
    return glfwWindowShouldClose(m_glfw_window);
}

void GLFWSurface::swap_buffers()
{
    glfwSwapBuffers(m_glfw_window);
}

} // namespace omi_glfw
