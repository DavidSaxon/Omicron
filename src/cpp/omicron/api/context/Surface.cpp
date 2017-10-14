#include "omicron/api/context/Surface.hpp"

#include "omicron/api/context/ContextSubsystem.hpp"


namespace omi
{
namespace context
{

//------------------------------------------------------------------------------
//                                    GLOBALS
//------------------------------------------------------------------------------

namespace
{

// the implementation object
static omi::SubsytemObject<Surface>* g_impl = nullptr;

} // namespace anonymous

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL Surface* Surface::instance()
{
    return g_impl->get();
}

OMI_API_GLOBAL void Surface::set_implementation(
        omi::SubsytemObject<Surface>* impl)
{
    g_impl = impl;
}

OMI_API_GLOBAL void Surface::destroy()
{
    if(g_impl != nullptr)
    {
        delete g_impl;
        g_impl = nullptr;
    }
}

//------------------------------------------------------------------------------
//                             PROTECTED CONSTRUCTOR
//------------------------------------------------------------------------------

OMI_API_GLOBAL Surface::Surface()
{
}

//------------------------------------------------------------------------------
//                              PROTECTED DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_GLOBAL Surface::~Surface()
{
}

} // namespace context
} // namespace omi
