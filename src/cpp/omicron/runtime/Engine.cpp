#include "omicron/runtime/Engine.hpp"

#include <omicron/subsystem/Input.hpp>
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

bool Engine::cycle()
{
    // TODO:
    return true;
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

    // get the subsystem manager
    SubsystemManager* ss_manager = SubsystemManager::get_instance();

    // get the window manager subsystem
    omi::ss::WindowManager* ss_window_manager =
        dynamic_cast<omi::ss::WindowManager*>(
            ss_manager->get_subsystem(omi::ss::Subsystem::kRoleWindowManager)
        );
    assert(ss_window_manager != nullptr);
    // get the input subsystem
    omi::ss::Input* ss_input = dynamic_cast<omi::ss::Input*>(
        ss_manager->get_subsystem(omi::ss::Subsystem::kRoleInput)
    );
    assert(ss_input != nullptr);

    // TODO: move this somewhere...
    ss_window_manager->set_mode(omi::ss::WindowManager::kModeFullscreen);

    global::logger->info << "Starting main loop" << std::endl;
    // start the main loop
    ss_input->start_main_loop(&Engine::cycle);

    if(!omi::runtime::boot::shutdown_routine())
    {
        omi::runtime::boot::get_critical_stream()
            << "Engine startup failed. Aborting" << std::endl;
        return -1;
    }

    return 0;
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

Engine::Engine()
{
}

} // namespace runtime
} // namespace omi
