/*!
 * \file
 * \author David Saxon
 */
#ifndef OMI_GL_SUBSYSTEM_HPP_
#define OMI_GL_SUBSYSTEM_HPP_

#include <omicron/subsystem/Renderer.hpp>

/*!
 * \brief TODO
 */
class GLSubsystem : public omi::ss::Renderer
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    GLSubsystem();

    //--------------------------------------------------------------------------
    //                                DESTRUCTORS
    //--------------------------------------------------------------------------

    virtual ~GLSubsystem();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // override
    virtual void startup();
};

#endif
