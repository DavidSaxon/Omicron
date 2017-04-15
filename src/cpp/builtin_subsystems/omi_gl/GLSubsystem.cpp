#include "omi_gl/GLSubsystem.hpp"

// TODO: REMOVE ME
// #include <iostream>
// #include <omicron/Proto.hpp>

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
}
