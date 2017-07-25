#include "omicron/runtime/subsystem/WindowSubsystem.hpp"

#include <cassert>

#include <omicron/api/window/MainWindow.hpp>
#include <omicron/api/window/subsystem/WindowBootstrap.hpp>

#include "omicron/runtime/RuntimeGlobals.hpp"


namespace omi
{
namespace runtime
{
namespace ss
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

WindowSubsystem::WindowSubsystem()
    : m_bootstrapper       (nullptr)
    , m_main_window_factory(nullptr)
{
}

//------------------------------------------------------------------------------
//                                  DESTRUCTORS
//------------------------------------------------------------------------------

WindowSubsystem::~WindowSubsystem()
{
    assert(m_bootstrapper == nullptr);
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void WindowSubsystem::bind(arc::io::dl::Handle library)
{
    global::logger->debug << "Binding window subsystem." << std::endl;

    // get the window registration symbol from the library
    RegisterFunc* register_func = arc::io::dl::bind_symbol<RegisterFunc>(
        library,
        ARC_STRINGIFY_VALUE(OMICRON_API_WINDOW_SUBSYSTEM_REGISTER_SYMBOL)
    );

    // call the register function
    (*register_func)(
        (void**) &m_bootstrapper,
        (void**) &m_main_window_factory
    );

    // bind into the engine
    omi::window::MainWindow::set_host(m_main_window_factory);
}

void WindowSubsystem::startup()
{
    global::logger->debug << "Starting window subsystem." << std::endl;

    // start the window subsystem
    m_bootstrapper->startup();
}

void WindowSubsystem::release()
{
    global::logger->debug << "Releasing window subsystem." << std::endl;

    if(m_bootstrapper == nullptr)
    {
        return;
    }

    // shutdown
    m_bootstrapper->shutdown();

    // clear pointers (let the bootstrapper delete itself)
    m_bootstrapper = nullptr;
}

void WindowSubsystem::start_main_loop(
        omi::window::ss::EngineCycleFunc* engine_cycle_func)
{
    assert(m_bootstrapper != nullptr);
    m_bootstrapper->start_main_loop(engine_cycle_func);
}

} // namespace ss
} // namespace runtime
} // namespace omi
