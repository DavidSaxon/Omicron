/*!
 * \file
 * \author David Saxon
 */
#ifndef PXTRACE_FRAMEBUFFER_HPP_
#define PXTRACE_FRAMEBUFFER_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/gm/Vector.hpp>

#include <GL/glew.h>
#include <GL/gl.h>


/*!
 * \brief Manages and renders the pxtrace framebuffer.
 */
class FrameBuffer
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    FrameBuffer(const arc::gm::Vector2u& dimensions);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    ~FrameBuffer();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    void render();

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    arc::gm::Vector2u m_dimensions;

    GLuint m_vao;
    GLuint m_vertex_buffer;
    GLuint m_uv_buffer;
    GLuint m_shader_program;
    GLuint m_texture;

    std::vector<unsigned char> m_texture_data;

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    void init();

    void compile_shaders();
};

#endif
