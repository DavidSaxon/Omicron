/*!
 * \file
 * \author David Saxon
 */
#ifndef OMI_GL_SUBSYSTEM_HPP_
#define OMI_GL_SUBSYSTEM_HPP_

#include <oss/Renderer.hpp>

/*!
 * \brief TODO
 */
class GLSubsystem : public oss::Renderer
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     */
    GLSubsystem();

    //--------------------------------------------------------------------------
    //                                DESTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     */
    virtual ~GLSubsystem();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO:
     */
    virtual void boot();
};

#endif
