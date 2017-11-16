#include "omicron/api/context/Event.hpp"


namespace omi
{
namespace context
{

//------------------------------------------------------------------------------
//                                  DEFINITIONS
//------------------------------------------------------------------------------

//----------------------------E V E N T    N A M E S----------------------------

OMI_API_EXPORT const arc::str::UTF8String Event::kNameMouseMove = "mouse_move";

OMI_API_EXPORT const arc::str::UTF8String Event::kNameKeyPress   = "key_press";
OMI_API_EXPORT const arc::str::UTF8String Event::kNameKeyRelease =
    "key_release";

OMI_API_EXPORT const arc::str::UTF8String Event::kNameWindowResize =
    "window_resize";
OMI_API_EXPORT const arc::str::UTF8String Event::kNameWindowMove =
    "window_move";

//-----------------------------D A T A    N A M E S-----------------------------

OMI_API_EXPORT const arc::str::UTF8String Event::kDataMousePosition =
    "mouse_position";

OMI_API_EXPORT const arc::str::UTF8String Event::kDataKeyCode   = "key_code";
OMI_API_EXPORT const arc::str::UTF8String Event::kDataModifiers = "modifiers";

OMI_API_EXPORT const arc::str::UTF8String Event::kDataWindowSize =
    "window_size";
OMI_API_EXPORT const arc::str::UTF8String Event::kDataWindowPosition =
    "window_position";

OMI_API_EXPORT const arc::str::UTF8String Event::kNameEngineShutdown =
    "engine_shutdown";

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_EXPORT Event::Event(
        const arc::str::UTF8String& name,
        const omi::MapAttribute& data)
    : m_name(name)
    , m_data(data.as_immutable())
{
}

OMI_API_EXPORT Event::Event(const Event& other)
    : m_name(other.m_name)
    , m_data(other.m_data)
{
}

OMI_API_EXPORT Event::Event(Event&& other)
    : m_name(other.m_name)
    , m_data(other.m_data)
{
    other.m_name = "";
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
    m_name = other.m_name;
    m_data = other.m_data;

    return *this;
}

OMI_API_EXPORT Event& Event::operator=(Event&& other)
{
    m_name = other.m_name;
    m_data = other.m_data;

    other.m_name = "";
    other.m_data = omi::MapAttribute();

    return *this;
}

OMI_API_EXPORT bool Event::operator==(const Event& other) const
{
    return m_name == other.m_name && m_data == other.m_data;
}

OMI_API_EXPORT bool Event::operator!=(const Event& other) const
{
    return !((*this) == other);
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT bool Event::key_press(
            const omi::context::Event& event,
            omi::context::Event::KeyCode& key_code)
{
    if(event.get_name() != kNameKeyPress)
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

OMI_API_EXPORT bool Event::window_resize(
        const omi::context::Event& event,
        arc::int32& width,
        arc::int32& height)
{
    if(event.get_name() != kNameWindowResize)
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

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT const arc::str::UTF8String& Event::get_name() const
{
    return m_name;
}

OMI_API_EXPORT const omi::MapAttribute& Event::get_data() const
{
    return m_data;
}

} // namespace context
} // namespace omi
