/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_RUNTIME_SUBSYSTEM_RENDERSSDL_HPP_
#define OMICRON_RUNTIME_SUBSYSTEM_RENDERSSDL_HPP_

#include "omicron/runtime/subsystem/AbstractSSDL.hpp"


namespace omi
{
namespace runtime
{
namespace ss
{

/*!
 * \brief Subsystem Dynamic Library for binding the render subsystem.
 */
class RenderSSDL
    : public AbstractSSDL
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    RenderSSDL();

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~RenderSSDL();

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

    class RenderSSDLImpl;
    RenderSSDLImpl* m_impl;
};

} // namespace ss
} // namespace runtime
} // namespace omi

#endif
