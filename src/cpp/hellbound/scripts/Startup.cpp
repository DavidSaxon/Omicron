#include "omicron/api/GameInterface.hpp"

#include <omicron/api/context/Surface.hpp>
#include <omicron/api/report/Logging.hpp>
#include <omicron/api/res/ResourceRegistry.hpp>
#include <omicron/api/scene/SceneState.hpp>

#include "hellbound/HellboundGlobals.hpp"


namespace hell
{

bool startup_routine()
{
    // set up the logger
    global::logger =
        omi::report::log_handler.vend_input(arc::log::Profile("HELLBOUND"));

    global::logger->debug << "Running startup routine" << std::endl;

    try
    {
        // we want the mouse hidden and locked to the centre of the window
        omi::context::Surface::instance()->hide_cursor(true);
        omi::context::Surface::instance()->lock_mouse(true);
    }
    catch(const std::exception& exc)
    {
        global::logger->error
            << "Encountered exception during startup routine: "
            << exc.what() << std::endl;
        return false;
    }

    return true;
}

bool firstframe_routine()
{
    global::logger->debug << "Running first-frame routine" << std::endl;

    // TODO: TESTING
    try
    {
        omi::res::ResourceRegistry::instance()->load_blocking(
            omi::res::get_id("res/builtin/mesh/bunny.obj")
        );

        omi::scene::SceneState::instance().new_entity("hell_GameManager");
        omi::scene::SceneState::instance().new_entity("hell_ProtoEntity");
    }
    catch(const std::exception& exc)
    {
        global::logger->error
            << "Encountered exception during first-frame routine: "
            << exc.what() << std::endl;
        return false;
    }

    return true;
}

bool shutdown_routine()
{
    global::logger->debug << "Running shutdown routine" << std::endl;

    // remove the logger (NOTE: this shouldn't need to be done, but on Windows:
    // closing this DLL causes the memory for the logging input to be freed,
    // even though this DLL doesn't own it).
    omi::report::log_handler.remove_input(global::logger);

    return true;
}

} // namespace hell

OMI_GAME_STARTUP_ROUTINE(hell::startup_routine);
OMI_GAME_FIRSTFRAME_ROUTINE(hell::firstframe_routine);
OMI_GAME_SHUTDOWN_ROUTINE(hell::shutdown_routine);
