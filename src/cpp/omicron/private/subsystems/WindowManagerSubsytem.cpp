#include "omicron/private/subsystems/WindowManagerSubsytem.hpp"

#include "omicron/private/Logging.hpp"

namespace omi_
{
namespace subsys
{

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

WindowManagerSubsystem* WindowManagerSubsystem::get_instance()
{
    static WindowManagerSubsystem singleton;
    return &singleton;
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

WindowManagerSubsystem::~WindowManagerSubsystem()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

bool WindowManagerSubsystem::startup(arc::io::dl::Handle dl_handle)
{
    omi_::logger->debug
        << "Binding and initialising window manager subsystem." << std::endl;

    // TODO:

    return true;
}

bool WindowManagerSubsystem::shutdown()
{
    // TODO:

    return true;
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTORS
//------------------------------------------------------------------------------

WindowManagerSubsystem::WindowManagerSubsystem()
    : m_initialised(false)
    , dl_handle    (nullptr)
{
}

} // namespace subsys
} // namespace omi_
