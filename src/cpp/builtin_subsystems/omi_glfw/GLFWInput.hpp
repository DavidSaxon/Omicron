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
