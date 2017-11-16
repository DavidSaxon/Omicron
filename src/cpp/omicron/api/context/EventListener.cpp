#include "omicron/api/context/EventListener.hpp"

#include "omicron/api/context/EventService.hpp"


namespace omi
{
namespace context
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT EventListener::EventListener()
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT EventListener::~EventListener()
{
    unsubsribe_from_all_events();
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT void EventListener::on_event(const omi::context::Event& event)
{
    // do nothing by default
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT void EventListener::subscribe_to_event(
        const arc::str::UTF8String& name)
{
    // already subscribed?
    auto f_subscribed = m_subscribed_events.find(name);
    if(f_subscribed != m_subscribed_events.end())
    {
        // TODO: could warn here?
        return;
    }

    omi::context::EventService::instance().subscribe(this, name);
    m_subscribed_events.insert(name);
}

OMI_API_EXPORT void EventListener::unsubsribe_from_event(
        const arc::str::UTF8String& name)
{
    // not subscribed?
    auto f_subscribed = m_subscribed_events.find(name);
    if(f_subscribed == m_subscribed_events.end())
    {
        // TODO: could warn here?
        return;
    }

    omi::context::EventService::instance().unsubscribe(this, name);
    m_subscribed_events.erase(f_subscribed);
}

OMI_API_EXPORT void EventListener::unsubsribe_from_all_events()
{
    for(const arc::str::UTF8String& name : m_subscribed_events)
    {
        omi::context::EventService::instance().unsubscribe(this, name);
    }
    m_subscribed_events.clear();
}

} // namespace context
} // namespace omi
