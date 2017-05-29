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

class QTBootstrap : public omi::window::ss::Bootstrap
{
public:

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~QTBootstrap();

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the Omi QT bootstrapper.
     */
    static QTBootstrap* get_instance();

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

    QTBootstrap();
};

} // namespace omi_qt

#endif
