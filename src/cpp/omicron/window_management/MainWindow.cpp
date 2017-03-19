#include "omicron/window_management/MainWindow.hpp"

namesapce omi
{
namespace wm
{

//------------------------------------------------------------------------------
//                               STATIC ATTRIBUTES
//------------------------------------------------------------------------------

bool MainWindow::s_initialised = false;

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

MainWindow* MainWindow::get_instance()
{
    // has window management been initialised yet?
    // TODO:
}

} // namespace wm
} // namespace omi
