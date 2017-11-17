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
        const arc::str::UTF8String& type)
{
    // already subscribed?
    auto f_subscribed = m_subscribed_events.find(type);
    if(f_subscribed != m_subscribed_events.end())
    {
        // TODO: could warn here?
        return;
    }

    omi::context::EventService::instance().subscribe(this, type);
    m_subscribed_events.insert(type);
}

OMI_API_EXPORT void EventListener::unsubsribe_from_event(
        const arc::str::UTF8String& type)
{
    // not subscribed?
    auto f_subscribed = m_subscribed_events.find(type);
    if(f_subscribed == m_subscribed_events.end())
    {
        // TODO: could warn here?
        return;
    }

    omi::context::EventService::instance().unsubscribe(this, type);
    m_subscribed_events.erase(f_subscribed);
}

OMI_API_EXPORT void EventListener::unsubsribe_from_all_events()
{
    for(const arc::str::UTF8String& type : m_subscribed_events)
    {
        omi::context::EventService::instance().unsubscribe(this, type);
    }
    m_subscribed_events.clear();
}

} // namespace context
} // namespace omi
