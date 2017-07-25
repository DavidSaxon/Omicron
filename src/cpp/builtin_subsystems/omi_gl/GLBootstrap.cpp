#include "omi_gl/GLBootstrap.hpp"

#include <omicron/api/report/Logging.hpp>

#include "omi_gl/GLGlobals.hpp"


namespace omi_gl
{

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

GLBootstrap::~GLBootstrap()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

 GLBootstrap* GLBootstrap::get_instance()
 {
    static GLBootstrap instance;
    return &instance;
 }

 //-----------------------------------------------------------------------------
 //                            PUBLIC STATIC FUNCTIONS
 //-----------------------------------------------------------------------------

void GLBootstrap::startup()
{
    // set up logging
    global::logger =
        omi::report::log_handler.vend_input(arc::log::Profile("OMICRON-GL"));

    global::logger->info << "Starting Omicron GL subsystem." << std::endl;

    // super call
    omi::render::ss::Bootstrap::startup();
}

void GLBootstrap::shutdown()
{
    global::logger->info << "Shutting down Omicron GL subsystem." << std::endl;

    // super call
    omi::render::ss::Bootstrap::shutdown();
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTORS
//------------------------------------------------------------------------------

GLBootstrap::GLBootstrap()
    : omi::render::ss::Bootstrap()
{
}

} // namespace omi_gl
