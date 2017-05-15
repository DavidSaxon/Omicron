#include "pxtrace/PXSubsystem.hpp"

#include <GL/glew.h>
#include <GL/gl.h>

// TODO: REMOVE ME
#include <iostream>

//------------------------------------------------------------------------------
//                               REGISTER SUBSYSTEM
//------------------------------------------------------------------------------

OSS_REGISTER_SUBSYSTEM(PXSubsystem);

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

PXSubsystem::PXSubsystem()
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
    // TODO:
    glewInit();
}

void PXSubsystem::setup_rendering()
{
    // TODO:
    glClearColor(1.0F, 0.2F, 0.1F, 1.0F);
}

void PXSubsystem::render()
{
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // TODO:

    std::cout << "PX RENDER" << std::endl;
}
