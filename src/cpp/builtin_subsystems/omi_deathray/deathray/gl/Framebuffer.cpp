/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/gl/Framebuffer.hpp"

#include <arcanecore/lx/Alignment.hpp>

#include "deathray/gl/Texture2D.hpp"

namespace death
{
namespace gl
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class Framebuffer::FramebufferImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the id of the texture
    GLuint m_id;

    // the current resolution of the framebuffer
    arc::lx::Vector2u m_resolution;

    // the colour component texture
    GLuint m_colour_component;
    // the depth component
    GLuint m_depth_component;

public:

    ARC_LX_ALIGNED_NEW;

    //--------------------------C O N S T R U C T O R---------------------------

    FramebufferImpl()
        : m_id              (0)
        , m_resolution      (0, 0)
        , m_colour_component(0)
        , m_depth_component (0)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~FramebufferImpl()
    {
        release();
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    void init(const arc::lx::Vector2u& resolution)
    {
        release();
        m_resolution = resolution;

        // generate
        glGenFramebuffers(1, &m_id);
    }

    void release()
    {
        m_depth_component  = 0;
        m_colour_component = 0;
        if(m_id != 0)
        {
            glDeleteFramebuffers(1, &m_id);
            m_id = 0;
        }
    }

    void attach_colour_texture(const Texture2D& texture)
    {
        attach_colour_texture(texture.get_native());
    }

    void attach_colour_texture(GLuint texture)
    {
        // do nothing if not initialised
        if(m_id == 0)
        {
            // TODO: throw an error here (or a least report)
            return;
        }

        m_colour_component = texture;

        bind();
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            m_colour_component,
            0
        );
    }

    void attach_depth_texture(const Texture2D& texture)
    {
        attach_depth_texture(texture.get_native());
    }

    void attach_depth_texture(GLuint texture)
    {
        // do nothing if not initialised
        if(m_id == 0)
        {
            // TODO: throw an error here (or a least report)
            return;
        }

        m_depth_component = texture;

        bind();
        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D,
            m_depth_component,
            0
        );
    }

    void bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    }

    void unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLuint get_native() const
    {
        return m_id;
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

DEATH_API_EXPORT Framebuffer::Framebuffer()
    : m_impl(new FramebufferImpl())
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

DEATH_API_EXPORT Framebuffer::~Framebuffer()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

DEATH_API_EXPORT void Framebuffer::init(const arc::lx::Vector2u& resolution)
{
    m_impl->init(resolution);
}

DEATH_API_EXPORT void Framebuffer::release()
{
    m_impl->release();
}

DEATH_API_EXPORT void Framebuffer::attach_colour_texture(
        const Texture2D& texture)
{
    m_impl->attach_colour_texture(texture);
}

DEATH_API_EXPORT void Framebuffer::attach_colour_texture(GLuint texture)
{
    m_impl->attach_colour_texture(texture);
}

DEATH_API_EXPORT void Framebuffer::attach_depth_texture(
        const Texture2D& texture)
{
    m_impl->attach_depth_texture(texture);
}

DEATH_API_EXPORT void Framebuffer::attach_depth_texture(GLuint texture)
{
    m_impl->attach_depth_texture(texture);
}

DEATH_API_EXPORT void Framebuffer::bind()
{
    m_impl->bind();
}

DEATH_API_EXPORT void Framebuffer::unbind()
{
    m_impl->unbind();
}

DEATH_API_EXPORT GLuint Framebuffer::get_native() const
{
    return m_impl->get_native();
}

} // namespace gl
} // namespace death
