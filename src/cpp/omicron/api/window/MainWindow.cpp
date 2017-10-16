#include "omicron/api/window/MainWindow.hpp"

#include <cassert>


namespace omi
{
namespace window
{

//------------------------------------------------------------------------------
//                               STATIC ATTRIBUTES
//------------------------------------------------------------------------------

omi::window::ss::MainWindowFactory* MainWindow::s_factory_func = nullptr;

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT MainWindow::~MainWindow()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT MainWindow* MainWindow::instance()
{
    assert(s_factory_func != nullptr);

    static MainWindow* inst = s_factory_func();
    return inst;
}

OMI_API_EXPORT void MainWindow::set_host(
        omi::window::ss::MainWindowFactory* factory_func)
{
    s_factory_func = factory_func;
}

//------------------------------------------------------------------------------
//                             PROTECTED CONSTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT MainWindow::MainWindow()
{
}

} // namespace window
} // namespace omi
