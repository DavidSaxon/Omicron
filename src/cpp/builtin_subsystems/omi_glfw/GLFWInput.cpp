#include "omi_glfw/GLFWInput.hpp"

#include <omicron/api/context/EventService.hpp>

#include "omi_glfw/GLFWGlobals.hpp"


namespace omi_glfw
{
namespace input
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

void mouse_move_callback(GLFWwindow* window, double pos_x, double pos_y)
{
    // construct the data
    omi::Int32Attribute::ArrayType mouse_position = {
        static_cast<arc::int32>(pos_x),
        static_cast<arc::int32>(pos_y)
    };
    omi::MapAttribute::DataType data =
    {
        {
            omi::context::Event::kDataMousePosition,
            omi::Int32Attribute(mouse_position)
        }
    };

    // construct and broadcast the event to the engine
    omi::context::Event event(
        omi::context::Event::kTypeMouseMove,
        omi::MapAttribute(data)
    );
    omi::context::EventService::instance().broadcast(event);
}

void mouse_button_callback(
        GLFWwindow* window,
        int button,
        int action,
        int mods)
{
    // determine the event type
    arc::str::UTF8String type;
    switch(action)
    {
        case GLFW_PRESS:
            type = omi::context::Event::kTypeMouseButtonPress;
            break;
        case GLFW_RELEASE:
            type = omi::context::Event::kTypeMouseButtonRelease;
            break;
        default:
            // not interested in this event
            return;
    }

    // construct the data
    omi::MapAttribute::DataType data =
    {
        {
            omi::context::Event::kDataKeyCode,
            // can safely cast GLFW mouse buttons to Omicron mouse buttons for
            // now since they align
            omi::Int32Attribute(button)
        },
        {
            omi::context::Event::kDataModifiers,
            // can safely cast GLFW modifier flags to Omicron modifier flags for
            // now since they align
            omi::Int32Attribute(mods)
        }
    };

    // construct and broadcast the event to the engine
    omi::context::Event event(type, omi::MapAttribute(data));
    omi::context::EventService::instance().broadcast(event);
}

void mouse_scroll_callback(GLFWwindow* window, double amount_x, double amount_y)
{
    // construct the data
    omi::MapAttribute::DataType data =
    {
        {
            omi::context::Event::kDataMouseScrollAmountX,
            omi::Int32Attribute(static_cast<arc::int32>(amount_x))
        },
        {
            omi::context::Event::kDataMouseScrollAmountY,
            omi::Int32Attribute(static_cast<arc::int32>(amount_y))
        }
    };

    // construct and broadcast the event to the engine
    omi::context::Event event(
        omi::context::Event::kTypeMouseScroll,
        omi::MapAttribute(data)
    );
    omi::context::EventService::instance().broadcast(event);
}

void mouse_enter_callback(GLFWwindow* window, int entered)
{
    // determine the type
    arc::str::UTF8String type;
    if(entered)
    {
        type = omi::context::Event::kTypeMouseEnter;
    }
    else
    {
        type = omi::context::Event::kTypeMouseExit;
    }
    // construct and broadcast the event to the engine
    omi::context::Event event(type, omi::MapAttribute());
    omi::context::EventService::instance().broadcast(event);
}

void key_callback(
        GLFWwindow* window,
        int key,
        int scancode,
        int action,
        int mods)
{
    // determine the event type
    arc::str::UTF8String type;
    switch(action)
    {
        case GLFW_PRESS:
            type = omi::context::Event::kTypeKeyPress;
            break;
        case GLFW_RELEASE:
            type = omi::context::Event::kTypeKeyRelease;
            break;
        default:
            // not interested in this event
            return;
    }

    // construct the data
    omi::MapAttribute::DataType data =
    {
        {
            omi::context::Event::kDataKeyCode,
            // can safely cast GLFW keys code to Omicron key codes for now since
            // they align
            omi::Int32Attribute(key)
        },
        {
            omi::context::Event::kDataModifiers,
            // can safely cast GLFW modifier flags to Omicron modifier flags for
            // now since they align
            omi::Int32Attribute(mods)
        }
    };

    // construct and broadcast the event to the engine
    omi::context::Event event(type, omi::MapAttribute(data));
    omi::context::EventService::instance().broadcast(event);
}

} // namespace input
} // namespace omi_glfw
