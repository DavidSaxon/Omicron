#include "omicron/api/window/MainWindow.hpp"


namespace omi
{
namespace window
{

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

MainWindow::~MainWindow()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

MainWindow* MainWindow::get_instance()
{
    static MainWindow instance;
    return &instance;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

// TODO:
void MainWindow::set_mode(WindowMode mode)
{
    // TODO: pass through the subsystem
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

MainWindow::MainWindow()
{
}

} // namespace window
} // namespace omi
