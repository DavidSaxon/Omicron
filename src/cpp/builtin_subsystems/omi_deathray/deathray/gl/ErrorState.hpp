/*!
 * \file
 * \author David Saxon
 *
 * \utilities for handling OpenGL errors.
 */
#ifndef DEATHRAY_GL_ERRORSTATE_HPP_
#define DEATHRAY_GL_ERRORSTATE_HPP_

#include <arcanecore/base/str/UTF8String.hpp>

#include <GL/glew.h>

#include "deathray/DeathRayAPI.hpp"


namespace death
{
namespace gl
{
namespace error
{

/*!
 * \brief Returns the string representation of the given error code.
 */
DEATH_API_EXPORT arc::str::UTF8String errorcode_to_string(GLenum error_code);

/*!
 * \brief Checks the graphics state for any GL errors and reports them
 *
 * \oaram action The action that was being performed before this error check
 *               (will be include)
 */
DEATH_API_EXPORT void check_state(const arc::str::UTF8String& action = "");

} // namespace error
} // namespace gl
} // namespace death

#endif
