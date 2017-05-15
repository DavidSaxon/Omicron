/*!
 * \file
 * \author David Saxon
 */
#ifndef OMI_QT_SUBSYSTEM_HPP_
#define OMI_QT_SUBSYSTEM_HPP_

#include <omicron/subsystem/Input.hpp>
#include <omicron/subsystem/UI.hpp>
#include <omicron/subsystem/WindowManager.hpp>

#include "omi_qt/MainWindow.hpp"

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class QApplication;

/*!
 * \brief TODO
 */
class QTSubsystem
    : public omi::ss::WindowManager
    , public omi::ss::Input
    , public omi::ss::UI
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    QTSubsystem();

    //--------------------------------------------------------------------------
    //                                DESTRUCTORS
    //--------------------------------------------------------------------------

    virtual ~QTSubsystem()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    virtual void startup();

    // override
    virtual void shutdown();

    // override
    virtual void set_mode(WindowMode mode);

    // override
    virtual void start_main_loop(omi::ss::Input::EngineCycleFunc engine_cycle);

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief The Qt application being used.
     */
    QApplication* m_qt_application;

    /*!
     * \brief The instance of the main window.
     */
    MainWindow* m_main_window;

    /*!
     * \brief The window mode being used.
     */
    WindowMode m_mode;
};

#endif
