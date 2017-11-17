/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_CONTEXT_EVENTSERVICE_HPP_
#define OMICRON_API_CONTEXT_EVENTSERVICE_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/base/str/UTF8String.hpp>

#include "omicron/api/API.hpp"
#include "omicron/api/context/Event.hpp"


namespace omi
{
namespace context
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class EventListener;


/*!
 * \brief The EventService is a singleton that manages the broadcasting and
 *        propagation of events within Omicron.
 */
class EventService
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
protected:

    //--------------------------------------------------------------------------
    //                                  FRIENDS
    //--------------------------------------------------------------------------

    friend class EventListener;

public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the Omicron EventSystem.
     */
    OMI_API_EXPORT static EventService& instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Broadcasts the given event to all EventListener who are
     *        subscribed to events with the same type.
     */
    OMI_API_EXPORT void broadcast(const omi::context::Event& event);

    /*!
     * \brief Utility function that broadcasts a shutdown event to the engine.
     */
    OMI_API_EXPORT void broadcast_shutdown();

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Subscribes the given EventListener to have its on_event function
     *        called every time an event with the given function is broadcast.
     */
    OMI_API_EXPORT void subscribe(
            omi::context::EventListener* listener,
            const arc::str::UTF8String& type);

    /*!
     * \brief Unsubscribes the given EventListener from events with the given
     *        type.
     */
    OMI_API_EXPORT void unsubscribe(
            omi::context::EventListener* listener,
            const arc::str::UTF8String& type);

private:

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTOR
    //--------------------------------------------------------------------------

    EventService();

    //--------------------------------------------------------------------------
    //                             PRIVATE DESTRUCTOR
    //--------------------------------------------------------------------------

    ~EventService();

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class EventServiceImpl;
    EventServiceImpl* m_impl;
};

} // namespace context
} // namespace omi

#endif
