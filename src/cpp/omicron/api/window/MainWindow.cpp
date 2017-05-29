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
    // TODO: use factory
    return nullptr;

    // static MainWindow instance;
    // return &instance;
}

//------------------------------------------------------------------------------
//                             PROTECTED CONSTRUCTOR
//------------------------------------------------------------------------------

MainWindow::MainWindow()
{
}

} // namespace window
} // namespace omi
