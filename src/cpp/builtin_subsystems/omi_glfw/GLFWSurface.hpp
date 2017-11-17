/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_GLFW_SURFACE_HPP_
#define OMICRON_GLFW_SURFACE_HPP_

#include <arcanecore/lx/Alignment.hpp>
#include <arcanecore/lx/Vector.hpp>

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

    ARC_LX_ALIGNED_NEW;

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

    virtual arc::int32 get_width() const override;

    virtual arc::int32 get_height() const override;

    virtual arc::int32 get_position_x() const override;

    virtual arc::int32 get_position_y() const override;

    virtual void hide_cursor(bool state) override;

    virtual void lock_mouse(bool state) override;

    /*!
     * \brief Returns the pointer to the GLFW window object.
     */
    GLFWwindow* get_native();

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

    /*!
     * \brief Is called as the update cycle is ending.
     */
    void cycle_end();

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    // the GLFW window instance
    GLFWwindow* m_glfw_window;

    // the size of the window (in pixels)
    arc::lx::Vector2i m_size;
    // the position of the window (in pixels)
    arc::lx::Vector2i m_position;

    // whether the cursor is currently hidden or not
    bool m_hide_cursor;
    // whether the mouse is currently locked or not
    bool m_lock_mouse;

    //--------------------------------------------------------------------------
    //                          PRIVATE STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    // The Omicron static Surface instance casted to a GLFWSurface type.
    static GLFWSurface* glfw_instance();

    // Is called when the window size changes.
    static void resize_callback(GLFWwindow* window, int width, int height);

    // Is called when the window is moved.
    static void move_callback(GLFWwindow* window, int pos_x, int pos_y);

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // connects the callbacks registered on this window to Omicron's event
    // system
    void connect_callbacks();
};

} // namespace omi_glfw

#endif
