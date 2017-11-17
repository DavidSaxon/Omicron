#include "omicron/api/context/Event.hpp"


namespace omi
{
namespace context
{

//------------------------------------------------------------------------------
//                                  DEFINITIONS
//------------------------------------------------------------------------------

//----------------------------E V E N T    N A M E S----------------------------

OMI_API_EXPORT const arc::str::UTF8String Event::kTypeMouseMove = "mouse_move";
OMI_API_EXPORT const arc::str::UTF8String Event::kTypeMouseButtonPress =
    "mouse_button_press";
OMI_API_EXPORT const arc::str::UTF8String Event::kTypeMouseButtonRelease =
    "mouse_button_release";
OMI_API_EXPORT const arc::str::UTF8String Event::kTypeMouseScroll =
    "mouse_scroll";
OMI_API_EXPORT const arc::str::UTF8String Event::kTypeMouseEnter =
    "mouse_enter";
OMI_API_EXPORT const arc::str::UTF8String Event::kTypeMouseExit =
    "mouse_exit";

OMI_API_EXPORT const arc::str::UTF8String Event::kTypeKeyPress   = "key_press";
OMI_API_EXPORT const arc::str::UTF8String Event::kTypeKeyRelease =
    "key_release";

OMI_API_EXPORT const arc::str::UTF8String Event::kTypeWindowResize =
    "window_resize";
OMI_API_EXPORT const arc::str::UTF8String Event::kTypeWindowMove =
    "window_move";

//-----------------------------D A T A    N A M E S-----------------------------

OMI_API_EXPORT const arc::str::UTF8String Event::kDataModifiers = "modifiers";

OMI_API_EXPORT const arc::str::UTF8String Event::kDataMousePosition =
    "mouse_position";
OMI_API_EXPORT const arc::str::UTF8String Event::kDataMouseButton =
    "mouse_button";
OMI_API_EXPORT const arc::str::UTF8String Event::kDataMouseScrollAmountX =
    "amount_x";
OMI_API_EXPORT const arc::str::UTF8String Event::kDataMouseScrollAmountY =
    "amount_";

OMI_API_EXPORT const arc::str::UTF8String Event::kDataKeyCode = "key_code";

OMI_API_EXPORT const arc::str::UTF8String Event::kDataWindowSize =
    "window_size";
OMI_API_EXPORT const arc::str::UTF8String Event::kDataWindowPosition =
    "window_position";

OMI_API_EXPORT const arc::str::UTF8String Event::kTypeEngineShutdown =
    "engine_shutdown";

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_EXPORT Event::Event(
        const arc::str::UTF8String& type,
        const omi::MapAttribute& data)
    : m_type(type)
    , m_data(data.as_immutable())
{
}

OMI_API_EXPORT Event::Event(const Event& other)
    : m_type(other.m_type)
    , m_data(other.m_data)
{
}

OMI_API_EXPORT Event::Event(Event&& other)
    : m_type(other.m_type)
    , m_data(other.m_data)
{
    other.m_type = "";
    other.m_data = omi::MapAttribute();
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT Event::~Event()
{
}

//------------------------------------------------------------------------------
//                                   OPERATORS
//------------------------------------------------------------------------------

OMI_API_EXPORT Event& Event::operator=(const Event& other)
{
    m_type = other.m_type;
    m_data = other.m_data;

    return *this;
}

OMI_API_EXPORT Event& Event::operator=(Event&& other)
{
    m_type = other.m_type;
    m_data = other.m_data;

    other.m_type = "";
    other.m_data = omi::MapAttribute();

    return *this;
}

OMI_API_EXPORT bool Event::operator==(const Event& other) const
{
    return m_type == other.m_type && m_data == other.m_data;
}

OMI_API_EXPORT bool Event::operator!=(const Event& other) const
{
    return !((*this) == other);
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT bool Event::mouse_move(
        const omi::context::Event& event,
        arc::int32& position_x,
        arc::int32& position_y)
{
    if(event.get_type() != kTypeMouseMove)
    {
        return false;
    }

    // get the position
    omi::Int32Attribute position_attr = event.get_data()[kDataMousePosition];
    if(!position_attr.is_valid() || position_attr.get_size() != 2)
    {
        return false;
    }

    position_x  = position_attr.at(0);
    position_y = position_attr.at(1);
    return true;
}

OMI_API_EXPORT bool Event::mouse_button_press(
        const omi::context::Event& event,
        omi::context::Event::MouseButton& button)
{
    if(event.get_type() != kTypeMouseButtonPress)
    {
        return false;
    }

    // get the key code
    omi::Int32Attribute button_attr = event.get_data()[kDataMouseButton];
    if(!button_attr.is_valid() || button_attr.get_size() != 1)
    {
        return false;
    }

    button =
        static_cast<omi::context::Event::MouseButton>(button_attr.get_value());
    return true;
}

OMI_API_EXPORT bool Event::mouse_button_press(
        const omi::context::Event& event,
        omi::context::Event::MouseButton& button,
        omi::context::Event::Modifier& modifiers)
{
    if(event.get_type() != kTypeMouseButtonPress)
    {
        return false;
    }

    // get the key code
    omi::Int32Attribute button_attr = event.get_data()[kDataMouseButton];
    if(!button_attr.is_valid() || button_attr.get_size() != 1)
    {
        return false;
    }

    // get the modifiers
    omi::Int32Attribute modifiers_attr = event.get_data()[kDataModifiers];
    if(!modifiers_attr.is_valid() || modifiers_attr.get_size() != 1)
    {
        return false;
    }

    button =
        static_cast<omi::context::Event::MouseButton>(button_attr.get_value());
    modifiers =
        static_cast<omi::context::Event::Modifier>(modifiers_attr.get_value());
    return true;
}

OMI_API_EXPORT bool Event::mouse_button_release(
        const omi::context::Event& event,
        omi::context::Event::MouseButton& button)
{
    if(event.get_type() != kTypeMouseButtonRelease)
    {
        return false;
    }

    // get the key code
    omi::Int32Attribute button_attr = event.get_data()[kDataMouseButton];
    if(!button_attr.is_valid() || button_attr.get_size() != 1)
    {
        return false;
    }

    button =
        static_cast<omi::context::Event::MouseButton>(button_attr.get_value());
    return true;
}

OMI_API_EXPORT bool Event::mouse_button_release(
        const omi::context::Event& event,
        omi::context::Event::MouseButton& button,
        omi::context::Event::Modifier& modifiers)
{
    if(event.get_type() != kTypeMouseButtonRelease)
    {
        return false;
    }

    // get the key code
    omi::Int32Attribute button_attr = event.get_data()[kDataMouseButton];
    if(!button_attr.is_valid() || button_attr.get_size() != 1)
    {
        return false;
    }

    // get the modifiers
    omi::Int32Attribute modifiers_attr = event.get_data()[kDataModifiers];
    if(!modifiers_attr.is_valid() || modifiers_attr.get_size() != 1)
    {
        return false;
    }

    button =
        static_cast<omi::context::Event::MouseButton>(button_attr.get_value());
    modifiers =
        static_cast<omi::context::Event::Modifier>(modifiers_attr.get_value());
    return true;
}

OMI_API_EXPORT bool Event::mouse_scroll(
        const omi::context::Event& event,
        arc::int32& amount_x,
        arc::int32& amount_y)
{
    if(event.get_type() != kTypeMouseScroll)
    {
        return false;
    }

    // get the x amount
    omi::Int32Attribute amount_x_attr =
        event.get_data()[kDataMouseScrollAmountX];
    if(!amount_x_attr.is_valid() || amount_x_attr.get_size() != 1)
    {
        return false;
    }
    // get the y amount
    omi::Int32Attribute amount_y_attr =
        event.get_data()[kDataMouseScrollAmountY];
    if(!amount_y_attr.is_valid() || amount_y_attr.get_size() != 1)
    {
        return false;
    }

    amount_x = amount_x_attr.get_value();
    amount_y = amount_y_attr.get_value();
    return true;
}

OMI_API_EXPORT bool Event::key_press(
        const omi::context::Event& event,
        omi::context::Event::KeyCode& key_code)
{
    if(event.get_type() != kTypeKeyPress)
    {
        return false;
    }

    // get the key code
    omi::Int32Attribute key_code_attr = event.get_data()[kDataKeyCode];
    if(!key_code_attr.is_valid() || key_code_attr.get_size() != 1)
    {
        return false;
    }

    key_code =
        static_cast<omi::context::Event::KeyCode>(key_code_attr.get_value());
    return true;
}

OMI_API_EXPORT bool Event::key_press(
            const omi::context::Event& event,
            omi::context::Event::KeyCode& key_code,
            omi::context::Event::Modifier& modifiers)
{
    if(event.get_type() != kTypeKeyPress)
    {
        return false;
    }

    // get the key code
    omi::Int32Attribute key_code_attr = event.get_data()[kDataKeyCode];
    if(!key_code_attr.is_valid() || key_code_attr.get_size() != 1)
    {
        return false;
    }

    // get the modifiers
    omi::Int32Attribute modifiers_attr = event.get_data()[kDataModifiers];
    if(!modifiers_attr.is_valid() || modifiers_attr.get_size() != 1)
    {
        return false;
    }

    key_code =
        static_cast<omi::context::Event::KeyCode>(key_code_attr.get_value());
    modifiers =
        static_cast<omi::context::Event::Modifier>(modifiers_attr.get_value());
    return true;
}

OMI_API_EXPORT bool Event::key_release(
        const omi::context::Event& event,
        omi::context::Event::KeyCode& key_code)
{
    if(event.get_type() != kTypeKeyRelease)
    {
        return false;
    }

    // get the key code
    omi::Int32Attribute key_code_attr = event.get_data()[kDataKeyCode];
    if(!key_code_attr.is_valid() || key_code_attr.get_size() != 1)
    {
        return false;
    }

    key_code =
        static_cast<omi::context::Event::KeyCode>(key_code_attr.get_value());
    return true;
}

OMI_API_EXPORT bool Event::key_release(
        const omi::context::Event& event,
        omi::context::Event::KeyCode& key_code,
        omi::context::Event::Modifier& modifiers)
{
    if(event.get_type() != kTypeKeyRelease)
    {
        return false;
    }

    // get the key code
    omi::Int32Attribute key_code_attr = event.get_data()[kDataKeyCode];
    if(!key_code_attr.is_valid() || key_code_attr.get_size() != 1)
    {
        return false;
    }

    // get the modifiers
    omi::Int32Attribute modifiers_attr = event.get_data()[kDataModifiers];
    if(!modifiers_attr.is_valid() || modifiers_attr.get_size() != 1)
    {
        return false;
    }

    key_code =
        static_cast<omi::context::Event::KeyCode>(key_code_attr.get_value());
    modifiers =
        static_cast<omi::context::Event::Modifier>(modifiers_attr.get_value());
    return true;
}

OMI_API_EXPORT bool Event::window_resize(
        const omi::context::Event& event,
        arc::int32& width,
        arc::int32& height)
{
    if(event.get_type() != kTypeWindowResize)
    {
        return false;
    }

    // get the height and width
    omi::Int32Attribute size_attr = event.get_data()[kDataWindowSize];
    if(!size_attr.is_valid() || size_attr.get_size() != 2)
    {
        return false;
    }

    width  = size_attr.at(0);
    height = size_attr.at(1);
    return true;
}

OMI_API_EXPORT bool Event::window_move(
        const omi::context::Event& event,
        arc::int32& position_x,
        arc::int32& position_y)
{
    if(event.get_type() != kTypeWindowMove)
    {
        return false;
    }

    // get the position
    omi::Int32Attribute position_attr = event.get_data()[kDataWindowPosition];
    if(!position_attr.is_valid() || position_attr.get_size() != 2)
    {
        return false;
    }

    position_x = position_attr.at(0);
    position_y = position_attr.at(1);
    return true;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT const arc::str::UTF8String& Event::get_type() const
{
    return m_type;
}

OMI_API_EXPORT const omi::MapAttribute& Event::get_data() const
{
    return m_data;
}

} // namespace context
} // namespace omi
