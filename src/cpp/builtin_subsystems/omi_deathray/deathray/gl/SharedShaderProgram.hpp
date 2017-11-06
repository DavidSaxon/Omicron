/*!
 * \file
 * \author David Saxon
 */
#ifndef DEATHRAY_GL_SHAREDSHADERPROGRAM_HPP_
#define DEATHRAY_GL_SHAREDSHADERPROGRAM_HPP_

#include <GL/glew.h>

#include "deathray/DeathRayAPI.hpp"


namespace death
{
namespace gl
{

/*!
 * \brief Object which holds a reference counted ShaderProgram.
 */
class SharedShaderProgram
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:
};

} // namespace gl
} // namespace death

#endif
