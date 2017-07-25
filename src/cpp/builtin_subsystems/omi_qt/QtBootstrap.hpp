/*!
 * \file
 * \author David Saxon
 */
#ifndef OMI_QT_BOOTSTRAP_HPP_
#define OMI_QT_BOOTSTRAP_HPP_

#include <omicron/api/window/subsystem/WindowBootstrap.hpp>


//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class QApplication;

namespace omi_qt
{

/*!
 * \brief Handles startup and shutdown of the Omicron Qt window subsystem.
 */
class QtBootstrap : public omi::window::ss::Bootstrap
{
public:

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~QtBootstrap();

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the Omicron Qt bootstrapper.
     */
    static QtBootstrap* get_instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    virtual void startup();

    // override
    virtual void shutdown();

    // override
    virtual void start_main_loop(
            omi::window::ss::EngineCycleFunc* engine_cycle_func);

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The Qt application being used.
     */
    QApplication* m_qt_application;

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTORS
    //--------------------------------------------------------------------------

    QtBootstrap();
};

} // namespace omi_qt

#endif
