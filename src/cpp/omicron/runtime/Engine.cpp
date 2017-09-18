#include "omicron/runtime/Engine.hpp"

#include <omicron/api/window/MainWindow.hpp>

#include "omicron/runtime/RuntimeGlobals.hpp"
#include "omicron/runtime/boot/BootRoutines.hpp"
#include "omicron/runtime/subsystem/SubsystemManager.hpp"


namespace omi
{
namespace runtime
{

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

Engine::~Engine()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

Engine* Engine::instance()
{
    static Engine inst;
    return &inst;
}

bool Engine::cycle_static()
{
    return Engine::instance()->cycle();
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

int Engine::execute()
{
    if(!omi::runtime::boot::startup_routine())
    {
        omi::runtime::boot::get_critical_stream()
            << "Engine startup failed. Aborting" << std::endl;
        return -1;
    }

    // TODO: need to do first time window setup somewhere...
    omi::window::MainWindow* main_window =
        omi::window::MainWindow::instance();
    main_window->set_mode(omi::window::kModeFullscreen);

    // get the subsystem manager
    ss::SubsystemManager* ss_manager = ss::SubsystemManager::instance();

    global::logger->info << "Starting main loop" << std::endl;
    // start the main loop
    ss_manager->start_main_loop(&Engine::cycle_static);

    m_setup = false;
    if(!omi::runtime::boot::shutdown_routine())
    {
        omi::runtime::boot::get_critical_stream()
            << "Engine startup failed. Aborting" << std::endl;
        return -1;
    }

    return 0;
}

bool Engine::cycle()
{
    // perform first time setup
    if(!m_setup)
    {
        global::logger->info << "Performing first-frame setup" << std::endl;
        try
        {
            // TODO: setup rendering
            // try
            // {
            //     m_renderer->setup_rendering();
            // }
            // catch(const std::exception& exc)
            // {
            //     global::logger->critical
            //         << "Renderer setup failed with error: " << exc.what()
            //         << std::endl;
            //     return false;
            // }

            // engine is now live! run routines
            if(!omi::runtime::boot::engine_live_routine())
            {
                return false;
            }

            m_setup = true;
        }
        catch(const std::exception& exc)
        {
            global::logger->critical
                << "Encountered exception during first-frame setup routines: "
                << exc.what() << std::endl;
            return false;
        }
    }

    // TODO:
    // render the frame
    // m_renderer->render();

    return true;
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

Engine::Engine()
    : m_setup(false)
{
}

} // namespace runtime
} // namespace omi
