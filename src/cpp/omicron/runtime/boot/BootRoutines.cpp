#include "omicron/runtime/boot/BootRoutines.hpp"

#include <arcanecore/base/Exceptions.hpp>
#include <arcanecore/base/Preproc.hpp>
#include <arcanecore/config/Document.hpp>
#include <arcanecore/config/visitors/Shorthand.hpp>

#include <omicron/api/asset/AssetLibrary.hpp>
#include <omicron/api/config/ConfigInline.hpp>
#include <omicron/api/report/ReportBoot.hpp>
#include <omicron/api/report/stats/StatsDatabase.hpp>
#include <omicron/api/report/stats/StatsQuery.hpp>
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
static void os_startup_routine();

/*!
 * \brief Performs reporting on shutdown.
 */
static void shutdown_reports();

/*!
 * \brief Prints stats to the logger.
 */
static void print_stats(const arc::io::sys::Path& query_path);

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
        shutdown_reports();
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

static void os_startup_routine()
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

        // TODO: REMOVE ME
        omi::res::ResourceRegistry::instance()->load_blocking(
            omi::res::get_id("res/builtin/mesh/bunny.obj")
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

static void shutdown_reports()
{
    // build the path to the configuration data for shutdown reporting
    arc::io::sys::Path config_path(omi::runtime::global::config_root_dir);
    config_path << "report" << "shutdown.json";

    // built-in memory data
    static const arc::str::UTF8String config_compiled(
        OMICRON_CONFIG_INLINE_RUNTIME_REPORT_SHUTDOWN
    );

    // construct the document
    arc::config::Document m_config_data(config_path, &config_compiled);

    // print stats?
    if(*m_config_data.get("print_stats.enable", AC_BOOLV))
    {
        // get the path to the query
        arc::io::sys::Path query_path =
            *m_config_data.get("print_stats.query_path", AC_PATHV);
        print_stats(query_path);
    }
}

static void print_stats(const arc::io::sys::Path& query_path)
{
    // build the query
    omi::report::StatsQuery query(query_path);
    // execute
    omi::report::StatsDatabase::instance()->execute_query(query);
    // dump the results
    // TODO:
}

} // namespace boot
} // namespace runtime
} // namespace omi
