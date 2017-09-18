#include "omi_gl/GLSubsystem.hpp"

#include <GL/glew.h>
#include <GL/gl.h>


//------------------------------------------------------------------------------
//                               REGISTER SUBSYSTEM
//------------------------------------------------------------------------------

OSS_REGISTER_SUBSYSTEM(GLSubsystem);

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

GLSubsystem::GLSubsystem()
{
    // TODO: setup logger

    // TODO: REMOVE ME
    // std::cout
    //     << "global var: " << omi::proto::global_var << std::endl;
    // std::cout
    //     << "func: " << omi::proto::test_me() << std::endl;
    // omi::proto::global_var = "OPENGL";
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

GLSubsystem::~GLSubsystem()
{

}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void GLSubsystem::startup()
{
    // TODO:
    glewInit();
}

void GLSubsystem::setup_rendering()
{
    // TODO:
    glClearColor(0.0F, 0.7F, 1.0F, 1.0F);

    // TODO: TESTING: REMOVE ME
}

void GLSubsystem::render()
{
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // TODO:
}
