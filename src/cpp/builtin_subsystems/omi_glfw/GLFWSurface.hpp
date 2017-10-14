/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_GLFW_SURFACE_HPP_
#define OMICRON_GLFW_SURFACE_HPP_

#include <omicron/api/context/Surface.hpp>


namespace omi_glfw
{

/*!
 * \brief The GLFW implementation of Omicron's context surface.
 */
class GLFWSurface
    : public omi::context::Surface
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    GLFWSurface();

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~GLFWSurface();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // TODO:
};

} // namespace omi_glfw

#endif
