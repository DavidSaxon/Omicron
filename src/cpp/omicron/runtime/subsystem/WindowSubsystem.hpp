/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_RUNTIME_SUBSYSTEM_WINDOWSUBSYSTEM_HPP_
#define OMICRON_RUNTIME_SUBSYSTEM_WINDOWSUBSYSTEM_HPP_

#include <omicron/api/window/subsystem/WindowInterface.hpp>

#include "omicron/runtime/subsystem/AbstractSubsystem.hpp"


namespace omi
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

namespace window
{
namespace ss
{
class Bootstrap;
} // namespace ss
} // namespace window

namespace runtime
{
namespace ss
{

/*!
 * \brief Holds the components that provide access to the bound window
 *        subsystem.
 */
class WindowSubsystem : public AbstractSubsystem
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    WindowSubsystem();

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~WindowSubsystem();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    virtual void bind(arc::io::dl::Handle library);

    // override
    virtual void startup();

    // override
    virtual void release();

    /*!
     * \brief Starts the main loop of Omicron via the window subsystem.
     */
    void start_main_loop(omi::window::ss::EngineCycleFunc* engine_cycle_func);

private:

    //--------------------------------------------------------------------------
    //                                  TYPEDEFS
    //--------------------------------------------------------------------------

    /*!
     * \brief The function type that is used to register window subsystems.
     */
    typedef void (RegisterFunc)(
            void** bootstrapper,
            void** main_window_factory_func);

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The bootstrapper for this window subsystem.
     */
    omi::window::ss::Bootstrap* m_bootstrapper;
    /*!
     * \brief The factory function to get the MainWindow instance.
     */
    omi::window::ss::MainWindowFactory* m_main_window_factory;
};

} // namespace ss
} // namespace runtime
} // namespace omi

#endif
