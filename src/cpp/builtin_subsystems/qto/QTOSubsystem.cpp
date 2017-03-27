#include "qto/QTOSubsystem.hpp"

// TODO: REMOVE ME
#include <iostream>

//------------------------------------------------------------------------------
//                               REGISTER SUBSYSTEM
//------------------------------------------------------------------------------

OSS_REGISTER_SUBSYSTEM(qto::QTOSubsystem);

namespace qto
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

QTOSubsystem::QTOSubsystem()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void QTOSubsystem::boot()
{
    std::cout << "QTO boot!" << std::endl;
}

void QTOSubsystem::open_main_window()
{
    std::cout << "QTO open main window" << std::endl;
}

} // namespace qto
