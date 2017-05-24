/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_WINDOW_MAINWINDOW_HPP_
#define OMICRON_API_WINDOW_MAINWINDOW_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>

#include "omicron/api/window/Window.hpp"


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

    ~MainWindow();

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the Omicron engine.
     */
    static MainWindow* get_instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO:
     */
    void set_mode(WindowMode mode);

    // TODO:
    // set mode
    // set size
    // set position
    // etc

private:

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTOR
    //--------------------------------------------------------------------------

    MainWindow();
};

} // namespace window
} // namespace omi

#endif
