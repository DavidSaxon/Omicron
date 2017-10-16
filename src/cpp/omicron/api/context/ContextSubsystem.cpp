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
static omi::SubsytemObject<ContextSubsystem>* g_impl = nullptr;

} // namespace anonymous

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT ContextSubsystem* ContextSubsystem::instance()
{
    return g_impl->get();
}

OMI_API_EXPORT void ContextSubsystem::set_implementation(
    omi::SubsytemObject<ContextSubsystem>* impl)
{
    g_impl = impl;
}

OMI_API_EXPORT void ContextSubsystem::destroy()
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

OMI_API_EXPORT ContextSubsystem::ContextSubsystem()
{
}

//------------------------------------------------------------------------------
//                              PROTECTED DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT ContextSubsystem::~ContextSubsystem()
{
}

} // namespace context
} // namespace omi
