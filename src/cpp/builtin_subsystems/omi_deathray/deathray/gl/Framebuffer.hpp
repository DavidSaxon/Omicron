/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_GL_FRAMEBUFFER_HPP_
#define DEATHRAY_GL_FRAMEBUFFER_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/lx/Vector.hpp>

#include <GL/glew.h>

#include "deathray/DeathRayAPI.hpp"


namespace death
{
namespace gl
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class Texture2D;

/*!
 * \brief A utility class that manages are framebuffer that can be used for
 *        rendering to a texture.
 */
class Framebuffer
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new uninitialised Framebuffer.
     */
    DEATH_API_EXPORT Framebuffer();

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    DEATH_API_EXPORT virtual ~Framebuffer();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Initialises this Framebuffer for the given resolution.
     *
     * \note This will release the current state if the buffer has been
     *       initialised.
     */
    DEATH_API_EXPORT void init(const arc::lx::Vector2u& resolution);

    /*!
     * \brief Releases the current state of this Framebuffer.
     */
    DEATH_API_EXPORT void release();

    /*!
     * \brief Attaches the given Texture2D as this Framebuffer's colour
     *        component.
     */
    DEATH_API_EXPORT void attach_colour_texture(const Texture2D& texture);

    /*!
     * \brief Attaches the given native texture id as this Framebuffer's colour
     *        component.
     */
    DEATH_API_EXPORT void attach_colour_texture(GLuint texture);

    /*!
     * \brief Attaches the given Texture2D as this Framebuffer's depth
     *        component.
     */
    DEATH_API_EXPORT void attach_depth_texture(const Texture2D& texture);

    /*!
     * \brief Attaches the given native texture id as this Framebuffer's depth
     *        component.
     */
    DEATH_API_EXPORT void attach_depth_texture(GLuint texture);

    /*!
     * \brief Makes this the current bound framebuffer.
     */
    DEATH_API_EXPORT void bind();

    /*!
     * \brief Unbinds the current framebuffer.
     */
    DEATH_API_EXPORT void unbind();

    /*!
     * \brief Returns the native OpenGL id of the framebuffer.
     */
    DEATH_API_EXPORT GLuint get_native() const;

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class FramebufferImpl;
    FramebufferImpl* m_impl;
};

} // namespace gl
} // namespace death

#endif
