/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_GLFW_SURFACE_HPP_
#define OMICRON_GLFW_SURFACE_HPP_

#include <omicron/api/context/Surface.hpp>

#include <GLFW/glfw3.h>


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

    /*!
     * \brief Opens the GLFW window.
     */
    bool open();

    /*!
     * \brief Returns whether the surface has been requested to be closed.
     */
    bool should_close();

    /*!
     * \brief Swaps the front and back OpenGL buffers of this surface.
     */
    void swap_buffers();

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    // the GLFW window instance
    GLFWwindow* m_glfw_window;
};

} // namespace omi_glfw

#endif
