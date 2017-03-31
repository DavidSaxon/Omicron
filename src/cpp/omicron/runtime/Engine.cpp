#include "omicron/runtime/Engine.hpp"

#include "omicron/runtime/base/Logging.hpp"
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

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

int Engine::execute()
{
    if(!omi::runtime::boot::startup_routine())
    {
        omi::runtime::logging::get_critical_stream()
            << "Engine startup failed. Aborting." << std::endl;
        return -1;
    }

    // // TODO:

    if(!omi::runtime::boot::shutdown_routine())
    {
        omi::runtime::logging::get_critical_stream()
            << "Engine startup failed. Aborting." << std::endl;
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
