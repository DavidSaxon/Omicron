#include "omicron/private/boot/BootRoutines.hpp"

#include <arcanecore/base/Exceptions.hpp>

#include "omicron/private/Logging.hpp"

namespace omi
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


//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

void startup_routine()
{
    // warn and do nothing if Omicron has already been initialised
    if(initialised)
    {
        // TODO: warn through logger
        return;
    }

    try
    {
        logging::init_routine();
    }
    catch(const arc::ex::ArcException& exc)
    {
        // TODO: warn
    }
    catch(const std::exception& exc)
    {
        // TODO: warn
    }

    // Omicron has successfully started up
    initialised = true;
}

void shutdown_routine()
{
    // Omicron has successfully shutdown
    initialised = false;
}

} // namespace boot
} // namespace omi
