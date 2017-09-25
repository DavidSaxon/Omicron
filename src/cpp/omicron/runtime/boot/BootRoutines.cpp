#include "omicron/runtime/boot/BootRoutines.hpp"

#include <arcanecore/base/Exceptions.hpp>
#include <arcanecore/base/Preproc.hpp>
#include <arcanecore/base/clock/ClockOperations.hpp>
#include <arcanecore/config/Document.hpp>
#include <arcanecore/config/visitors/Shorthand.hpp>

#include <omicron/api/asset/AssetLibrary.hpp>
#include <omicron/api/common/Attributes.hpp>
#include <omicron/api/config/ConfigInline.hpp>
#include <omicron/api/report/ReportBoot.hpp>
#include <omicron/api/report/stats/StatsDatabase.hpp>
#include <omicron/api/report/stats/StatsOperations.hpp>
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

// Whether Omicron has been initialised yet or not.
static bool g_initialised = false;
// Whether first frame setup has been performed yet or not
static bool g_first_frame_complete = false;

// The config document for engine startup
arc::config::DocumentPtr g_startup_config;

// the time in milliseconds since epoch that Omicron started
static arc::uint64 g_start_time;

// stats
static omi::StringAttribute g_stat_start_at           ("", false);
static omi::StringAttribute g_stat_end_at             ("", false);
static omi::Int64Attribute  g_stat_active_time        (0, false);
static omi::Int64Attribute  g_stat_core_startup_time  (0, false);
static omi::Int64Attribute  g_stat_time_to_first_frame(0, false);
static omi::Int64Attribute  g_stat_shutdown_time(0, false);

//------------------------------------------------------------------------------
//                                   PROTOTYPES
//------------------------------------------------------------------------------

// Initialises operating system specific functionality.
static void os_startup_routine();

// Boot routine that is called once the Engine has become live.
// Is called with a valid GL context
bool engine_live_routine();

// Performs startup reports
static void startup_reports();

// Performs reporting on shutdown.
static void shutdown_reports();

// Prints stats to the logger.
static void print_stats(
        const arc::io::sys::Path& query_path,
        const arc::str::UTF8String& title);

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

// Defines the boot statistics
static void define_statistics()
{
    omi::report::StatsDatabase::instance()->define_entry(
        "Lifecycle.Start At",
        g_stat_start_at,
        "The time at which this instance of Omicron was started."
    );
    omi::report::StatsDatabase::instance()->define_entry(
        "Lifecycle.End At",
        g_stat_end_at,
        "The time at which this instance of Omicron ended."
    );
    omi::report::StatsDatabase::instance()->define_entry(
        "Lifecycle.Active Time (ms)",
        g_stat_active_time,
        "The amount of time Omicron has been active for."
    );
    omi::report::StatsDatabase::instance()->define_entry(
        "Lifecycle.Startup.Core Time (ms)",
        g_stat_core_startup_time,
        "The time taken by startup of the core engine (including "
        "subsystem loading and startup)."
    );
    omi::report::StatsDatabase::instance()->define_entry(
        "Lifecycle.Startup.Time to First Frame (ms)",
        g_stat_time_to_first_frame,
        "The time taken until rendering of the first frame of Omicron "
        "begins (This time includes initial loading and setup requiring a "
        "GL context)."
    );
    omi::report::StatsDatabase::instance()->define_entry(
        "Lifecycle.Shutdown.Time (ms)",
        g_stat_shutdown_time,
        "The time taken by shutdown of the engine."
    );
}

bool startup_routine()
{
    // warn and do nothing if Omicron has already been initialised
    if(g_initialised)
    {
        global::logger->warning
            << "Attempted to run engine startup routines after the engine has "
            << "already successfully started." << std::endl;
        return true;
    }

    // record the time the engine started
    g_start_time = arc::clock::get_current_time();

    define_statistics();

    // stat the time the engine started
    g_stat_start_at.set_at(
        0,
        arc::clock::get_datetime(g_start_time, "%Y/%m/%d - %H:%M:%S")
    );

    // build the path to the configuration data for startup
    arc::io::sys::Path config_path(omi::runtime::global::config_root_dir);
    config_path << "boot" << "startup.json";

    // built-in memory data
    static const arc::str::UTF8String config_compiled(
        OMICRON_CONFIG_INLINE_RUNTIME_BOOT_STARTUP
    );

    // construct the document
    g_startup_config.reset(
        new arc::config::Document(config_path, &config_compiled)
    );

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

    // stat the time of core startup
    g_stat_core_startup_time.set_at(
        0,
        arc::clock::get_current_time() - g_start_time
    );

    // Omicron has successfully started up
    g_initialised = true;
    return true;
}

static void os_startup_routine()
{
    global::logger->debug
        << "Initialising Operating System specific functionality." << std::endl;

    #ifdef ARC_OS_WINDOWS

        SetErrorMode(SEM_FAILCRITICALERRORS);

    #endif
}

bool first_frame_routine()
{
    // TODO: should this be moved?
    // stat the active time
    g_stat_active_time.set_at(
        0,
        arc::clock::get_current_time() - g_start_time
    );

    // only do this once
    if(g_first_frame_complete)
    {
        return true;
    }

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
        if(!engine_live_routine())
        {
            return false;
        }
    }
    catch(const std::exception& exc)
    {
        global::logger->critical
            << "Encountered exception during first-frame setup "
            << "routines: " << exc.what() << std::endl;
        return false;
    }


    // stat the time to first frame
    g_stat_time_to_first_frame.set_at(
        0,
        arc::clock::get_current_time() - g_start_time
    );
    // also use for the current active time
    g_stat_active_time.set_at(0, g_stat_time_to_first_frame.at(0));

    // perform reports
    startup_reports();

    // clear the startup config since it won't be needed anymore
    g_startup_config.reset(nullptr);

    g_first_frame_complete = true;
    return true;
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

static void startup_reports()
{
    // print stats?
    if(*g_startup_config->get("print_stats.enable", AC_BOOLV))
    {
        // get the path to the query
        arc::io::sys::Path query_path =
            *g_startup_config->get("print_stats.query_path", AC_PATHV);
        print_stats(query_path, "Startup Statistics");
    }
}

bool shutdown_routine()
{
    // time shutdown starts
    arc::uint64 shutdown_start_time = arc::clock::get_current_time();

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

        // get the time of shutdown
        arc::uint64 end_time = arc::clock::get_current_time();

        // stat
        g_stat_end_at.set_at(
            0,
            arc::clock::get_datetime(end_time, "%Y/%m/%d - %H:%M:%S")
        );
        g_stat_active_time.set_at(0, end_time - g_start_time);
        g_stat_shutdown_time.set_at(0, end_time - shutdown_start_time);

        // perform shutdown reporting
        try
        {
            shutdown_reports();
        }
        catch(const std::exception& exc)
        {
            global::logger->critical
                << "Failed while performing shutdown reports: " << exc.what()
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
    g_initialised = false;
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

static void shutdown_reports()
{
    // build the path to the configuration data for shutdown reporting
    arc::io::sys::Path config_path(omi::runtime::global::config_root_dir);
    config_path << "boot" << "shutdown.json";

    // built-in memory data
    static const arc::str::UTF8String config_compiled(
        OMICRON_CONFIG_INLINE_RUNTIME_BOOT_SHUTDOWN
    );

    // construct the document
    arc::config::Document config_data(config_path, &config_compiled);

    // print stats?
    if(*config_data.get("print_stats.enable", AC_BOOLV))
    {
        // get the path to the query
        arc::io::sys::Path query_path =
            *config_data.get("print_stats.query_path", AC_PATHV);
        print_stats(query_path, "Shutdown Statistics");
    }
}

static void print_stats(
        const arc::io::sys::Path& query_path,
        const arc::str::UTF8String& title)
{
    // build the query
    omi::report::StatsQuery query(query_path);
    // execute
    omi::report::StatsDatabase::instance()->execute_query(query);
    // dump the results
    omi::report::print_stats_query(
        query,
        global::logger->notice,
        title
    );
}

} // namespace boot
} // namespace runtime
} // namespace omi
