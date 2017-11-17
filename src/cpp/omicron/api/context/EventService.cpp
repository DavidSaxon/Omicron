#include "omicron/api/context/EventService.hpp"

#include <unordered_map>
#include <unordered_set>

#include "omicron/api/context/EventListener.hpp"


namespace omi
{
namespace context
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class EventService::EventServiceImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // mapping from event type to the listeners that are subscribed to them
    std::unordered_map<
        arc::str::UTF8String,
        std::unordered_set<EventListener*>
    > m_subscribers;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    EventServiceImpl()
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~EventServiceImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    void broadcast(const omi::context::Event& event)
    {
        // anything subscribed to this event?
        auto f_subscriber = m_subscribers.find(event.get_type());
        if(f_subscriber != m_subscribers.end())
        {
            for(EventListener* listener : f_subscriber->second)
            {
                listener->on_event(event);
            }
        }
    }

    void broadcast_shutdown()
    {
        broadcast(omi::context::Event(
            omi::context::Event::kTypeEngineShutdown,
            omi::MapAttribute()
        ));
    }

    void subscribe(
            omi::context::EventListener* listener,
            const arc::str::UTF8String& type)
    {
        auto f_subscriber = m_subscribers.find(type);
        if(f_subscriber == m_subscribers.end())
        {
            m_subscribers.emplace(type, std::unordered_set<EventListener*>());
            f_subscriber = m_subscribers.find(type);
        }

        // is the listener already subscribed?
        auto f_listener = f_subscriber->second.find(listener);
        if(f_listener != f_subscriber->second.end())
        {
            return;
        }

        // subscribe!
        f_subscriber->second.insert(listener);
    }

    void unsubscribe(
            omi::context::EventListener* listener,
            const arc::str::UTF8String& type)
    {
        auto f_subscriber = m_subscribers.find(type);
        if(f_subscriber == m_subscribers.end())
        {
            return;
        }

        // is the listener subscribed?
        auto f_listener = f_subscriber->second.find(listener);
        if(f_listener == f_subscriber->second.end())
        {
            return;
        }

        f_subscriber->second.erase(f_listener);
    }
};

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT EventService& EventService::instance()
{
    static EventService inst;
    return inst;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT void EventService::broadcast(const omi::context::Event& event)
{
    m_impl->broadcast(event);
}

OMI_API_EXPORT void EventService::broadcast_shutdown()
{
    m_impl->broadcast_shutdown();
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT void EventService::subscribe(
        omi::context::EventListener* listener,
        const arc::str::UTF8String& type)
{
    m_impl->subscribe(listener, type);
}

OMI_API_EXPORT void EventService::unsubscribe(
        omi::context::EventListener* listener,
        const arc::str::UTF8String& type)
{
    m_impl->unsubscribe(listener, type);
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

EventService::EventService()
    : m_impl(new EventServiceImpl())
{
}

//------------------------------------------------------------------------------
//                               PRIVATE DESTRUCTOR
//------------------------------------------------------------------------------

EventService::~EventService()
{
    delete m_impl;
}

} // namespace context
} // namespace omi
