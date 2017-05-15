/*!
 * \file
 * \author David Saxon
 */
#ifndef PXTRACE_SUBSYSTEM_HPP_
#define PXTRACE_SUBSYSTEM_HPP_

#include <omicron/subsystem/Renderer.hpp>

/*!
 * \brief TODO
 */
class PXSubsystem : public omi::ss::Renderer
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    PXSubsystem();

    //--------------------------------------------------------------------------
    //                                DESTRUCTORS
    //--------------------------------------------------------------------------

    virtual ~PXSubsystem();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    virtual void startup();

    // override
    virtual void setup_rendering();

    // override
    virtual void render();
};

#endif
