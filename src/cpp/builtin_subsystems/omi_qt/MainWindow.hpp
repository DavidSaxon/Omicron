/*!
 * \file
 * \author David Saxon
 */
#ifndef OMI_QT_MAINWINDOW_HPP_
#define OMI_QT_MAINWINDOW_HPP_

#include <omicron/api/window/MainWindow.hpp>


/*!
 * \brief The Qt implementation of Omicron's main window.
 */
class MainWindow : public omi::window::MainWindow
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    MainWindow();

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~MainWindow();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    virtual WindowMode get_mode() const;

    // override
    virtual void set_mode(WindowMode mode);

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    virtual void open();

    // override
    virtual void close();
};

#endif
