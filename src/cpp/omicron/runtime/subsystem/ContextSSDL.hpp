/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_RUNTIME_SUBSYSTEM_CONTEXTSSDL_HPP_
#define OMICRON_RUNTIME_SUBSYSTEM_CONTEXTSSDL_HPP_

#include "omicron/runtime/subsystem/AbstractSSDL.hpp"


namespace omi
{
namespace runtime
{
namespace ss
{

class ContextSSDL
    : public AbstractSSDL
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    ContextSSDL();

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~ContextSSDL();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    virtual bool bind(const arc::io::sys::Path& path) override;

    virtual void release() override;

    virtual const arc::str::UTF8String& get_version() const override;

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class ContextSSDLImpl;
    ContextSSDLImpl* m_impl;
};

} // namespace ss
} // namespace runtime
} // namespace omi


#endif
