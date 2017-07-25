/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_WINDOW_SUBSYSTEM_BOOTSTRAP_HPP_
#define OMICRON_API_WINDOW_SUBSYSTEM_BOOTSTRAP_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "omicron/api/window/subsystem/WindowInterface.hpp"


namespace omi
{
namespace window
{
namespace ss
{

/*!
 * \brief Object used to bootstrap a window subsystem and enters Omicron's main
 *        loop.
 */
class Bootstrap
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Bootstrap()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Starts up this Omicron window subsystem.
     *
     * Other than the Bootstrap's constructor this will be the first call made
     * to this object, and will only be made once.
     */
    virtual void startup()
    {
        m_active = true;
    }

    /*!
     * \brief Starts up this Omicron window subsystem.
     *
     * Other than the Bootstrap's destructor this will be the last call made
     * to this object, and will only be made once.
     */
    virtual void shutdown()
    {
        m_active = false;
    }

    /*!
     * \brief Starts the main loop of Omicron which will be managed by window
     *        subsystem and will call the given function every cycle of the
     *        main loop.
     */
    virtual void start_main_loop(EngineCycleFunc* engine_cycle_func) = 0;

protected:

    //--------------------------------------------------------------------------
    //                           PROTECTED CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Super constructor for window subsystem bootstrappers.
     */
    Bootstrap()
        : m_active(false)
    {
    }

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief Whether the bootstrapper has been started up correctly and not yet
     *        shutdown.
     */
    bool m_active;
};

} // namespace ss
} // namespace window
} // namespace omi

#endif
