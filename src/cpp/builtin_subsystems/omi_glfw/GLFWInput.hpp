/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_GLFW_INPUT_HPP_
#define OMICRON_GLFW_INPUT_HPP_

#include <GLFW/glfw3.h>


namespace omi_glfw
{
namespace input
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief Is called by GLFW when the mouse moves.
 */
void mouse_move_callback(GLFWwindow* window, double pos_x, double pos_y);

/*!
 * \brief Is called when a mouse button is pressed.
 */
void mouse_button_callback(
        GLFWwindow* window,
        int button,
        int action,
        int mods);

/*!
 * \brief Is called when the mouse wheel is scrolled.
 */
void mouse_scroll_callback(
        GLFWwindow* window,
        double amount_x,
        double amount_y);

/*!
 * \brief Is called by GLFW when the mouse enter/leaves the window.
 */
void mouse_enter_callback(GLFWwindow* window, int entered);

/*!
 * \brief Is called by GLFW when a key is pressed or released.
 */
void key_callback(
        GLFWwindow* window,
        int key,
        int scancode,
        int action,
        int mods);

} // namespace input
} // namespace omi_glfw

#endif
