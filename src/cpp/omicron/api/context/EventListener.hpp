/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_CONTEXT_EVENTLISTENER_HPP_
#define OMICRON_API_CONTEXT_EVENTLISTENER_HPP_

#include <unordered_set>

#include <arcanecore/base/str/UTF8String.hpp>

#include "omicron/api/API.hpp"
#include "omicron/api/context/Event.hpp"


namespace omi
{
namespace context
{

/*!
 * \brief The EventListener class should be inherited from by an object that
 *         wishes to subscribe to Omicron events.
 */
class EventListener
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Super constructor.
     */
    OMI_API_EXPORT EventListener();

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual ~EventListener();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Is called when a event that this object is subscribed to is
     *        broadcast.
     */
    OMI_API_EXPORT virtual void on_event(const omi::context::Event& event);

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    OMI_API_EXPORT void subscribe_to_event(const arc::str::UTF8String& type);

    OMI_API_EXPORT void unsubsribe_from_event(const arc::str::UTF8String& type);

    OMI_API_EXPORT void unsubsribe_from_all_events();

private:

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // holds the names of the events this listener is subscribed to
    std::unordered_set<arc::str::UTF8String> m_subscribed_events;
};

} // namespace context
} // namespace omi

#endif
