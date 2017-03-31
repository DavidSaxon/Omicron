#include "omicron/runtime/boot/BootRoutines.hpp"

#include <arcanecore/base/Exceptions.hpp>
#include <arcanecore/base/Preproc.hpp>

#include "omicron/runtime/base/Logging.hpp"
#include "omicron/runtime/base/SubsystemManager.hpp"

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
        omi::runtime::logger->warning
            << "Attempted to run engine startup routines after the engine has "
            << "already successfully started." << std::endl;
        return true;
    }

    try
    {
        omi::runtime::logging::startup_routine();
        os_startup_routine();
        omi::runtime::SubsystemManager::get_instance()->startup();
    }
    // TODO: can we abuse the runtime exception and naming trick like Katana...
    catch(const arc::ex::ArcException& exc)
    {
        omi::runtime::logging::get_critical_stream()
            << "Encountered exception during engine startup routines: ["
            << exc.get_type() << "] \"" << exc.get_message() << "\""
            << std::endl;
        return false;
    }
    catch(const std::exception& exc)
    {
        omi::runtime::logging::get_critical_stream()
            << "Encountered exception during engine startup routines: \""
            << exc.what() << "\"" << std::endl;
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
        omi::runtime::SubsystemManager::get_instance()->shutdown();
    }
    // TODO: can we abuse the runtime exception and naming trick like Katana...
    catch(const arc::ex::ArcException& exc)
    {
        omi::runtime::logging::get_critical_stream()
            << "Encountered exception during engine shutdown routines: ["
            << exc.get_type() << "] \"" << exc.get_message() << "\""
            << std::endl;
        return false;
    }
    catch(const std::exception& exc)
    {
        omi::runtime::logging::get_critical_stream()
            << "Encountered exception during engine shutdown routines: \""
            << exc.what() << "\"" << std::endl;
        return false;
    }

    // Omicron has successfully shutdown
    initialised = false;
    return true;
}

void os_startup_routine()
{
    omi::runtime::logger->debug
        << "Initialising Operating System specific functionality." << std::endl;

    #ifdef ARC_OS_WINDOWS

        SetErrorMode(SEM_FAILCRITICALERRORS);

    #endif
}

} // namespace boot
} // namespace runtime
} // namespace omi
