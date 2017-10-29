/*!
 * \file
 * \author David Saxon
 */
#ifndef DEATHRAY_GL_SHADERPROGRAM_HPP_
#define DEATHRAY_GL_SHADERPROGRAM_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/base/str/UTF8String.hpp>
#include <arcanecore/io/sys/Path.hpp>
#include <arcanecore/lx/Matrix.hpp>
#include <arcanecore/lx/Vector.hpp>

#include <GL/glew.h>

#include "deathray/DeathRayAPI.hpp"


namespace death
{
namespace gl
{

/*!
 * \brief A utility class that handles compiling, binding, and sending data
 *        to a shader program.
 */
class ShaderProgram
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new uncompiled program with no shaders attached.
     */
    DEATH_API_EXPORT ShaderProgram();

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    DEATH_API_EXPORT virtual ~ShaderProgram();

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Converts the given shader type to a string representation.
     */
    DEATH_API_EXPORT static arc::str::UTF8String shader_type_to_string(
            GLenum shader_type);

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Attaches and compiles the shader source within given string.
     *
     * \note Requires link() to be called to actually generate a program with
     *       this shader.
     *
     * \param shader_type Specifies the type of shader to be created.
     * \param source The string containing the source code of the shader.
     *
     * \throw arc::ex::ParseError If the shader could not be parsed as a glsl
     *                            shader.
     */
    DEATH_API_EXPORT void attach_shader_from_string(
            GLenum shader_type,
            const arc::str::UTF8String& source);

    /*!
     * \brief Attaches and compiles the shader source within the given file.
     *
     * \note Requires link() to be called to actually generate a program with
     *       this shader.
     *
     * \param shader_type Specifies the type of shader to be created.
     * \param source The path the file containing the source code of the shader.
     *
     * \throw arc::ex::IOError If the file cannot be opened.
     * \throw arc::ex::ParseError If the shader could not be parsed as a glsl
     *                            shader.
     */
    DEATH_API_EXPORT void attach_shader_from_file(
            GLenum shader_type,
            const arc::io::sys::Path& source);

    /*!
     * \brief Links the currently attached and compiled shaders into a single
     *        program.
     *
     * \throw arc::ex::GLError If there is a problem linking the shaders into
     *                         the program.
     */
    DEATH_API_EXPORT void link();

    /*!
     * \brief Releases the current shader program (if it has been successfully
     *        link).
     */
    DEATH_API_EXPORT void release();

    /*!
     * \brief Binds this as the current shader program.
     */
    DEATH_API_EXPORT void bind();

    /*!
     * \brief Unbinds the current shader program.
     */
    DEATH_API_EXPORT void unbind();

    /*!
     * \brief Returns the native OpenGL id of the shader program.
     */
    DEATH_API_EXPORT GLuint get_native() const;

    /*!
     * \brief Returns the location of an uniform variable within this shader
     *        program or -1 if the location does not exist.
     */
    DEATH_API_EXPORT GLint get_uniform_location(
            const arc::str::UTF8String& location);

    /*!
     * \brief Passes the given signed int as an uniform to this shader program.
     *
     * \param location The location of the uniform to set.
     * \param value The value to set at the uniform location.
     */
    DEATH_API_EXPORT void set_uniform_1i(
            const arc::str::UTF8String& location,
            arc::int32 value);

    /*!
     * \brief Passes the given unsigned int as an uniform to this shader program.
     *
     * \param location The location of the uniform to set.
     * \param value The value to set at the uniform location.
     */
    DEATH_API_EXPORT void set_uniform_1u(
            const arc::str::UTF8String& location,
            arc::uint32 value);

    /*!
     * \brief Passes the given float as an uniform to this shader program.
     *
     * \param location The location of the uniform to set.
     * \param value The value to set at the uniform location.
     */
    DEATH_API_EXPORT void set_uniform_1f(
            const arc::str::UTF8String& location,
            float value);

    /*!
     * \brief Passes the given Vector3f as an uniform to this shader program.
     *
     * \param location The location of the uniform to set.
     * \param value The value to set at the uniform location.
     */
    DEATH_API_EXPORT void set_uniform_3f(
            const arc::str::UTF8String& location,
            const arc::lx::Vector3f& value);

    /*!
     * \brief Passes the given Matrix44f as an uniform to this shader program.
     *
     * \param location The location of the uniform to set.
     * \param value The value to set at the uniform location.
     */
    DEATH_API_EXPORT void set_uniform_44f(
            const arc::str::UTF8String& location,
            const arc::lx::Matrix44f& value);

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class ShaderProgramImpl;
    ShaderProgramImpl* m_impl;
};

} // namespace gl
} // namespace death

#endif
