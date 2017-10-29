#include "omicron/api/render/RenderSubsystem.hpp"


namespace omi
{
namespace render
{

//------------------------------------------------------------------------------
//                                    GLOBALS
//------------------------------------------------------------------------------

namespace
{

// the implementation object
static omi::SubsytemObject<RenderSubsystem>* g_impl = nullptr;

} // namespace anonymous

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------


OMI_API_EXPORT RenderSubsystem& RenderSubsystem::instance()
{
    return *g_impl->get();
}

OMI_API_EXPORT void RenderSubsystem::set_implementation(
        omi::SubsytemObject<RenderSubsystem>* impl)
{
    g_impl = impl;
}

OMI_API_EXPORT void RenderSubsystem::destroy()
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

OMI_API_EXPORT RenderSubsystem::RenderSubsystem()
{
}

//------------------------------------------------------------------------------
//                              PROTECTED DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT RenderSubsystem::~RenderSubsystem()
{
}

} // namespace render
} // namespace omi
