/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_WINDOW_MAINWINDOW_HPP_
#define OMICRON_API_WINDOW_MAINWINDOW_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "omicron/api/API.hpp"
#include "omicron/api/window/Window.hpp"
#include "omicron/api/window/subsystem/WindowInterface.hpp"


namespace omi
{
namespace window
{

/*!
 * \brief Singleton object which controls the main window of Omicron.
 */
class MainWindow
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT ~MainWindow();

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the main window.
     */
    OMI_API_EXPORT static MainWindow* instance();

    // hide from doxygen
    #ifndef IN_DOXYGEN

    OMI_API_EXPORT static void set_host(
            omi::window::ss::MainWindowFactory* factory_func);

    #endif
    // IN_DOXYGEN

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the current windowing mode of the main window.
     */
    virtual WindowMode get_mode() const = 0;

    /*!
     * \brief Sets the windowing mode the main window will use.
     */
    virtual void set_mode(WindowMode mode) = 0;

    // TODO:
    // set mode
    // set size
    // set position
    // etc

    // TODO: is minimized

    // TODO: minimise

    // TODO: expand

    // TODO: is focused

    // TODO: focus

protected:

    //--------------------------------------------------------------------------
    //                           PROTECTED CONSTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT MainWindow();

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

private:

    //--------------------------------------------------------------------------
    //                         PRIVATE STATIC ATTRIBUTES
    //--------------------------------------------------------------------------

    static omi::window::ss::MainWindowFactory* s_factory_func;
};

} // namespace window
} // namespace omi

#endif
