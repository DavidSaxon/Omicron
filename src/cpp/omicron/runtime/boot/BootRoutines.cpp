#include "omicron/runtime/boot/BootRoutines.hpp"

#include <arcanecore/base/Exceptions.hpp>
#include <arcanecore/base/Preproc.hpp>

#include <omicron/api/asset/AssetLibrary.hpp>
#include <omicron/api/report/ReportBoot.hpp>
#include <omicron/api/res/ResourceRegistry.hpp>

#include "omicron/runtime/RuntimeGlobals.hpp"
#include "omicron/runtime/boot/BootLogging.hpp"
#include "omicron/runtime/subsystem/SubsystemManager.hpp"

#ifdef ARC_OS_WINDOWS
    #include <windows.h>
#endif


namespace omi
{
namespace runtime
{
namespace boot
{

//------------------------------------------------------------------------------
//                                   VARIABLES
//------------------------------------------------------------------------------

/*!
 * \brief Whether Omicron has been initialised yet or not.
 */
static bool initialised = false;

//------------------------------------------------------------------------------
//                                   PROTOTYPES
//------------------------------------------------------------------------------

/*!
 * \brief Initialises operating system specific functionality.
 */
void os_startup_routine();

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

bool startup_routine()
{
    // warn and do nothing if Omicron has already been initialised
    if(initialised)
    {
        global::logger->warning
            << "Attempted to run engine startup routines after the engine has "
            << "already successfully started." << std::endl;
        return true;
    }

    try
    {
        omi::report::startup_routine();
        omi::runtime::boot::startup_logging_subroutine();
        os_startup_routine();
        if(!omi::res::ResourceRegistry::instance()->startup_routine())
        {
            global::logger->critical
                << "Failed during startup routines of the ResourceRegistry"
                << std::endl;
            return false;
        }
        omi::asset::AssetLibrary::instance()->startup_routine();
        omi::runtime::ss::SubsystemManager::instance()->startup();
    }
    catch(const std::exception& exc)
    {
        get_critical_stream()
            << "Encountered exception during engine startup routines: "
            << exc.what() << std::endl;
        return false;
    }

    // Omicron has successfully started up
    initialised = true;
    return true;
}

bool shutdown_routine()
{
    try
    {
        bool failure = false;
        omi::runtime::ss::SubsystemManager::instance()->shutdown();
        omi::asset::AssetLibrary::instance()->shutdown_routine();
        if(!omi::res::ResourceRegistry::instance()->shutdown_routine())
        {
            global::logger->critical
                << "Failed during shutdown routines of the ResourceRegistry"
                << std::endl;
            failure = true;
        }
        omi::runtime::boot::startup_logging_subroutine();
        omi::report::shutdown_routine();

        // shutdown failed at some point
        if(failure)
        {
            return false;
        }
    }
    catch(const std::exception& exc)
    {
        get_critical_stream()
            << "Encountered exception during engine shutdown routines: "
            << exc.what() << std::endl;
        return false;
    }

    // Omicron has successfully shutdown
    initialised = false;
    return true;
}

std::ostream& get_critical_stream()
{
    // return the from proper logging if input is not null
    if(global::logger != nullptr)
    {
        return global::logger->critical;
    }
    // return std::cerr
    std::cerr << "{OMICRON-RUNTIME} - [CRITICAL]: ";
    return std::cerr;
}

void os_startup_routine()
{
    global::logger->debug
        << "Initialising Operating System specific functionality." << std::endl;

    #ifdef ARC_OS_WINDOWS

        SetErrorMode(SEM_FAILCRITICALERRORS);

    #endif
}

bool engine_live_routine()
{
    try
    {
        // perform a multi-threaded load of the initial engine resources
        // TODO: should use some sort of "resource pack" to speicify what needs
        //       to be loaded
        // TODO: multi-threaded load function

        // TODO: can we move res?
        // TODO: REMOVE ME
        omi::asset::AssetLibrary::instance()->load_blocking(
            "res/builtin/mesh/bunny.obj"
        );
    }
    catch(const std::exception& exc)
    {
        get_critical_stream()
            << "Encountered exception during engine live routines: "
            << exc.what() << std::endl;
        return false;
    }


    return true;
}

} // namespace boot
} // namespace runtime
} // namespace omi
