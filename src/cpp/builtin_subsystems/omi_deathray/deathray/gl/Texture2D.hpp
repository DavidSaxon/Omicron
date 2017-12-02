/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_GL_TEXTURE2D_HPP_
#define DEATHRAY_GL_TEXTURE2D_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/lx/Vector.hpp>
#include <arcanecore/io/sys/Path.hpp>

#include <GL/glew.h>

#include "deathray/DeathRayAPI.hpp"


namespace death
{
namespace gl
{

/*!
 * \brief A utility for creating and setting the data of OpenGL 2-dimensional
 *        textures.
 */
class Texture2D
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new uninitialised 2-dimensional texture.
     */
    DEATH_API_EXPORT Texture2D();

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    DEATH_API_EXPORT virtual ~Texture2D();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Initliases the texture with the given data (can be null).
     *
     * \param internal The format of the OpenGL texture that will be created.
     * \param resolution The resolution of the texture (in texels).
     * \param format The format of the input data (e.g. GL_RGBA).
     * \param data_type The type that the input type is expressed in (e.g.
     *                  GL_UNSIGNED_BYTE).
     * \param The input data.
     */
    DEATH_API_EXPORT void init(
            GLint internal_format,
            const arc::lx::Vector2u& resolution,
            GLenum format,
            GLenum data_type,
            const GLvoid* data);

    // only support image loading in development builds
    #ifndef DEATH_API_MODE_PRODUCTION

    /*!
     * \brief Initializes the texture and loads the data from the given image
     *        file.
     */
    DEATH_API_EXPORT void load_from_file(const arc::io::sys::Path& file_path);

    #endif

    /*!
     * \brief Releases the current data of this texture.
     */
    DEATH_API_EXPORT void release();

    /*!
     * \brief Updates a region of the texture with new data.
     *
     * \param offset The offset (in texels) of the region of the texture to
     *               update.
     * \param size The width and height (in texels) or the region of the texture
     *              to update.
     * \param format The format of the input data (e.g. GL_RGBA).
     * \param data_type The type that the input type is expressed in (e.g.
     *                  GL_UNSIGNED_BYTE).
     * \param The input data.
     */
    DEATH_API_EXPORT void update(
            const arc::lx::Vector2u& offset,
            const arc::lx::Vector2u& size,
            GLenum format,
            GLenum data_type,
            const GLvoid* data);
    /*!
     * \brief Sets the minimize and magnify filtering that will be used by this
     *        texture.
     *
     * \note This function can be called before or after the init() function.
     */
    DEATH_API_EXPORT void set_filtering(GLenum min_filter, GLenum mag_filter);

    /*!
     * \brief Sets the the s and t texture wrapping modes.
     *
     * \note This function can be called before or after the init() function.
     */
    DEATH_API_EXPORT void set_wrap(GLenum wrap_s, GLenum wrap_t);

    /*!
     * \brief Makes this the current bound texture.
     */
    DEATH_API_EXPORT void bind();

    /*!
     * \brief Unbinds the current texture.
     */
    DEATH_API_EXPORT void unbind();

    /*!
     * \brief Returns the native OpenGL id of the texture.
     */
    DEATH_API_EXPORT GLuint get_native() const;

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class Texture2DImpl;
    Texture2DImpl* m_impl;
};

} // namespace gl
} // namespace death

#endif
