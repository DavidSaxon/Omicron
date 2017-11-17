#include "omi_glfw/GLFWSurface.hpp"

#include <omicron/api/context/EventService.hpp>

#include "omi_glfw/GLFWInput.hpp"


namespace omi_glfw
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

GLFWSurface::GLFWSurface()
    : omi::context::Surface()
    , m_glfw_window        (nullptr)
    , m_hide_cursor        (false)
    , m_lock_mouse         (false)
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

GLFWSurface::~GLFWSurface()
{
    if(m_glfw_window != nullptr)
    {
        glfwDestroyWindow(m_glfw_window);
    }
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

arc::int32 GLFWSurface::get_width() const
{
    return m_size(0);
}

arc::int32 GLFWSurface::get_height() const
{
    return m_size(1);
}

arc::int32 GLFWSurface::get_position_x() const
{
    return m_position(0);
}

arc::int32 GLFWSurface::get_position_y() const
{
    return m_position(1);
}

void GLFWSurface::hide_cursor(bool state)
{
    m_hide_cursor = state;

    // no window yet?
    if(m_glfw_window == nullptr)
    {
        return;
    }

    if(state)
    {
        glfwSetInputMode(m_glfw_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
        glfwSetInputMode(m_glfw_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void GLFWSurface::lock_mouse(bool state)
{
    m_lock_mouse = true;
}

GLFWwindow* GLFWSurface::get_native()
{
    return m_glfw_window;
}

bool GLFWSurface::open()
{
    // TODO: REMOVE ME
    m_size = arc::lx::Vector2i(1000, 1000);

    // TODO: where to get arguments from
    m_glfw_window = glfwCreateWindow(
        m_size(0),
        m_size(1),
        "DeathRay Development",
        nullptr,
        nullptr
    );
    if(m_glfw_window == nullptr)
    {
        return false;
    }

    glfwMakeContextCurrent(m_glfw_window);

    connect_callbacks();

    // set the cursor state
    hide_cursor(m_hide_cursor);

    // trigger the window callbacks
    resize_callback(m_glfw_window, m_size(0), m_size(1));
    move_callback(m_glfw_window, m_position(0), m_position(1));

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

void GLFWSurface::cycle_end()
{
    if(m_lock_mouse)
    {
        glfwSetCursorPos(m_glfw_window, m_size(0) / 2, m_size(1) / 2);
    }
}

//------------------------------------------------------------------------------
//                            PRIVATE STATIC FUNCTIONS
//------------------------------------------------------------------------------

GLFWSurface* GLFWSurface::glfw_instance()
{
    return static_cast<GLFWSurface*>(omi::context::Surface::instance());
}

void GLFWSurface::resize_callback(GLFWwindow* window, int width, int height)
{
    glfw_instance()->m_size = arc::lx::Vector2i(
        static_cast<arc::int32>(width),
        static_cast<arc::int32>(height)
    );

    // construct event data
    omi::Int32Attribute::ArrayType event_size = {
        static_cast<arc::int32>(width),
        static_cast<arc::int32>(height)
    };
    omi::MapAttribute::DataType data =
    {
        {
            omi::context::Event::kDataWindowSize,
            omi::Int32Attribute(event_size)
        }
    };
    // construct and broadcast the event to the engine
    omi::context::Event event(
        omi::context::Event::kTypeWindowResize,
        omi::MapAttribute(data)
    );
    omi::context::EventService::instance().broadcast(event);
}

void GLFWSurface::move_callback(GLFWwindow* window, int pos_x, int pos_y)
{
    glfw_instance()->m_position = arc::lx::Vector2i(
        static_cast<arc::int32>(pos_x),
        static_cast<arc::int32>(pos_y)
    );

    // construct event data
    omi::Int32Attribute::ArrayType event_position = {
        static_cast<arc::int32>(pos_x),
        static_cast<arc::int32>(pos_y)
    };
    omi::MapAttribute::DataType data =
    {
        {
            omi::context::Event::kDataWindowPosition,
            omi::Int32Attribute(event_position)
        }
    };
    // construct and broadcast the event to the engine
    omi::context::Event event(
        omi::context::Event::kTypeWindowMove,
        omi::MapAttribute(data)
    );
    omi::context::EventService::instance().broadcast(event);
}

//------------------------------------------------------------------------------
//                            PRIVATE MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void GLFWSurface::connect_callbacks()
{
    // connect window callbacks
    glfwSetWindowSizeCallback(m_glfw_window, &resize_callback);
    glfwSetWindowPosCallback(m_glfw_window, &move_callback);

    // connect input callbacks
    glfwSetCursorPosCallback(
        m_glfw_window,
        &omi_glfw::input::mouse_move_callback
    );
    glfwSetMouseButtonCallback(
        m_glfw_window,
        &omi_glfw::input::mouse_button_callback
    );
    glfwSetScrollCallback(
        m_glfw_window,
        &omi_glfw::input::mouse_scroll_callback
    );
    glfwSetCursorEnterCallback(
        m_glfw_window,
        &omi_glfw::input::mouse_enter_callback
    );
    glfwSetKeyCallback(
        m_glfw_window,
        &omi_glfw::input::key_callback
    );

}

} // namespace omi_glfw



