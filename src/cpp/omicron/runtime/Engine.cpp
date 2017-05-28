#include "omicron/runtime/Engine.hpp"

#include <omicron/subsystem/Input.hpp>
#include <omicron/subsystem/Renderer.hpp>
#include <omicron/subsystem/WindowManager.hpp>

#include "omicron/runtime/RuntimeGlobals.hpp"
#include "omicron/runtime/SubsystemManager.hpp"
#include "omicron/runtime/boot/BootRoutines.hpp"


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

Engine* Engine::get_instance()
{
    static Engine engine;
    return &engine;
}

bool Engine::cycle_static()
{
    return Engine::get_instance()->cycle();
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

    // // get the subsystem manager
    // SubsystemManager* ss_manager = SubsystemManager::get_instance();
    // // get the window manager subsystem
    // m_window_manager = dynamic_cast<omi::ss::WindowManager*>(
    //     ss_manager->get_subsystem(omi::ss::Subsystem::kRoleWindowManager)
    // );
    // assert(m_window_manager != nullptr);
    // // get the input subsystem
    // m_input = dynamic_cast<omi::ss::Input*>(
    //     ss_manager->get_subsystem(omi::ss::Subsystem::kRoleInput)
    // );
    // assert(m_input != nullptr);
    // // get the renderer subsystem
    // m_renderer = dynamic_cast<omi::ss::Renderer*>(
    //     ss_manager->get_subsystem(omi::ss::Subsystem::kRoleRenderer)
    // );
    // assert(m_renderer != nullptr);

    // // TODO: move this somewhere...
    // // m_window_manager->set_mode(omi::ss::WindowManager::kModeFullscreen);

    // global::logger->info << "Starting main loop" << std::endl;
    // // start the main loop
    // m_input->start_main_loop(&Engine::cycle_static);

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
            m_renderer->setup_rendering();
        }
        catch(const std::exception& exc)
        {
            global::logger->critical
                << "Renderer setup failed with error: " << exc.what()
                << std::endl;
            return false;
        }
        m_setup = true;
    }

    // render the frame
    m_renderer->render();

    return true;
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

Engine::Engine()
    : m_setup         (false)
    , m_window_manager(nullptr)
    , m_input         (nullptr)
    , m_renderer      (nullptr)
{
}

} // namespace runtime
} // namespace omi
