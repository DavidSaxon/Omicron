#include "omicron_runtime/Engine.hpp"

namespace omi_
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
    // if(!omi_::boot::startup_routine())
    // {
    //     (*omi_::boot::get_critical_stream())
    //         << "Engine startup failed. Aborting." << std::endl;
    //     return -1;
    // }

    // // TODO:

    // if(!omi_::boot::shutdown_routine())
    // {
    //     (*omi_::boot::get_critical_stream())
    //         << "Engine startup failed. Aborting." << std::endl;
    //     return -1;
    // }

    return 0;
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

Engine::Engine()
{
}

} // namespace omi_
