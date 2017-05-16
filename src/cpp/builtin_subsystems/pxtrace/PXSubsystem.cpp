#include "pxtrace/PXSubsystem.hpp"

#include <arcanecore/base/Exceptions.hpp>

#include <omicron/report/Logging.hpp>

#include "pxtrace/PXGlobals.hpp"


//------------------------------------------------------------------------------
//                               REGISTER SUBSYSTEM
//------------------------------------------------------------------------------

OSS_REGISTER_SUBSYSTEM(PXSubsystem);

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

PXSubsystem::PXSubsystem()
    : m_frame_buffer(nullptr)
{
    // TODO: setup logger
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

PXSubsystem::~PXSubsystem()
{

}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void PXSubsystem::startup()
{
    // set up logging
    global::logger =
        omi::report::log_handler.vend_input(arc::log::Profile("PXTRACE"));

    global::logger->info << "Starting up subsystem." << std::endl;
}

void PXSubsystem::shutdown()
{
    global::logger->info << "Shutting down subsystem." << std::endl;

    // remove the logger (NOTE: this shouldn't need to be done, but on Windows:
    // closing this DLL causes the memory for the logging input to be freed,
    // even though this DLL doesn't own it).
    omi::report::log_handler.remove_input(global::logger);
}

void PXSubsystem::setup_rendering()
{
    global::logger->debug << "Setting up rendering context" << std::endl;

    // TODO: need to be able to get the resolution

    // TODO:
    glewExperimental = GL_TRUE;
    glewInit();

    // get the OpenGL version
    GLint gl_major_version = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &gl_major_version);
    GLint gl_minor_version = 0;
    glGetIntegerv(GL_MINOR_VERSION, &gl_minor_version);

    global::logger->info
        << "Detected OpenGL version: " << gl_major_version << "."
        << gl_minor_version << std::endl;

    // check the version is 3.0+
    if(gl_major_version < 3)
    {
        arc::str::UTF8String error_message;
        error_message
            << "pxtrace requires an OpenGL context of 3.0 or greater. OpenGL "
            << "version " << gl_major_version << "." << gl_minor_version
            << " detected.";
        throw arc::ex::ContextError(error_message);
    }

    // create the frame buffer
    m_frame_buffer.reset(new FrameBuffer(arc::gm::Vector2u(480, 270)));
}

// TODO: shutdown rendering

void PXSubsystem::render()
{
    m_frame_buffer->render();
}
