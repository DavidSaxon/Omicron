/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/gl/Texture2D.hpp"

#include <arcanecore/base/Exceptions.hpp>

// only support image loading in development builds
#ifndef DEATH_API_MODE_PRODUCTION

#include <IL/il.h>

#endif


namespace death
{
namespace gl
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class Texture2D::Texture2DImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the id of the texture
    GLuint m_id;

    // filtering modes
    GLenum m_min_filter;
    GLenum m_mag_filter;
    // wrap modes
    GLenum m_wrap_s;
    GLenum m_wrap_t;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    Texture2DImpl()
        : m_id        (0)
        , m_min_filter(GL_NEAREST)
        , m_mag_filter(GL_NEAREST)
        , m_wrap_s    (GL_CLAMP)
        , m_wrap_t    (GL_CLAMP)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~Texture2DImpl()
    {
        release();
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    void init(
            GLint internal_format,
            const arc::lx::Vector2u& resolution,
            GLenum format,
            GLenum data_type,
            const GLvoid* data)
    {
        release();
        glEnable(GL_TEXTURE_2D);

        // generate
        glGenTextures(1, &m_id);
        bind();
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            internal_format,
            resolution(0),
            resolution(1),
            0,
            format,
            data_type,
            data
        );
        unbind();

        // apply modes
        apply_filtering();
        apply_wrap();
    }

    // only support image loading in development builds
    #ifndef DEATH_API_MODE_PRODUCTION

    void load_from_file(const arc::io::sys::Path& file_path)
    {
        static bool initialised = false;
        if(!initialised)
        {
            ilInit();
            initialised = true;
        }

        ILuint image_id = 0;

        // generate and bind the image
        ilGenImages(1, &image_id);
        ilBindImage(image_id);

        // match the image origin to OpenGL's
        ilEnable(IL_ORIGIN_SET);
        ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

        //load the image
        ILboolean success = ilLoadImage(file_path.to_native().get_raw());

        // check that the image loading successfully
        if(!success)
        {
            ilBindImage(0);
            ilDeleteImages(1, &image_id);
            // TODO: detect error
            arc::str::UTF8String error_message;
            error_message << "Failed to open image: " << file_path;
            throw arc::ex::RuntimeError(error_message);
        }

        // get the data and ensure it actually contains something
        void* data = ilGetData();
        if (!data)
        {
            ilBindImage(0);
            ilDeleteImages(1, &image_id);
            // TODO: better exception?
            arc::str::UTF8String error_message;
            error_message << "Empty image data: " << file_path;
            throw arc::ex::RuntimeError(error_message);
        }

        //get the important parameters from the image
        int width  = ilGetInteger(IL_IMAGE_WIDTH);
        int height = ilGetInteger(IL_IMAGE_HEIGHT);
        int type   = ilGetInteger(IL_IMAGE_TYPE);
        int format = ilGetInteger(IL_IMAGE_FORMAT);

        // set the pixel store parameters
        glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
        glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
        glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // create the texture
        init(
            format,
            arc::lx::Vector2u(
                static_cast<arc::uint32>(width),
                static_cast<arc::uint32>(height)
            ),
            format,
            type,
            data
        );
    }

    #endif

    void release()
    {
        if(m_id != 0)
        {
            glDeleteTextures(1, &m_id);
            m_id = 0;
        }
    }

    void update(
            const arc::lx::Vector2u& offset,
            const arc::lx::Vector2u& size,
            GLenum format,
            GLenum data_type,
            const GLvoid* data)
    {
        bind();
        glTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            offset(0),
            offset(1),
            size(0),
            size(1),
            format,
            data_type,
            data
        );
        unbind();
    }

    void set_filtering(GLenum min_filter, GLenum mag_filter)
    {
        m_min_filter = min_filter;
        m_mag_filter = mag_filter;
        apply_filtering();
    }

    void set_wrap(GLenum wrap_s, GLenum wrap_t)
    {
        m_wrap_s = wrap_s;
        m_wrap_t = wrap_t;
        apply_wrap();
    }

    void bind()
    {
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

    void unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    GLuint get_native() const
    {
        return m_id;
    }

private:

    //------------P R I V A T E    M E M B E R    F U N C T I O N S-------------

    // applies filtering mode to this texture (if it is setup)
    void apply_filtering()
    {
        if(m_id == 0)
        {
            return;
        }

        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_min_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_mag_filter);
    }

    // applies wrap mode to this texture (if it is setup)
    void apply_wrap()
    {
        if(m_id == 0)
        {
            return;
        }

        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrap_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrap_t);
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

DEATH_API_EXPORT Texture2D::Texture2D()
    : m_impl(new Texture2DImpl())
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

DEATH_API_EXPORT Texture2D::~Texture2D()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

DEATH_API_EXPORT void Texture2D::init(
        GLint internal_format,
        const arc::lx::Vector2u& resolution,
        GLenum format,
        GLenum data_type,
        const GLvoid* data)
{
    m_impl->init(internal_format, resolution, format, data_type, data);
}

// only support image loading in development builds
#ifndef DEATH_API_MODE_PRODUCTION

DEATH_API_EXPORT void Texture2D::load_from_file(
        const arc::io::sys::Path& file_path)
{
    m_impl->load_from_file(file_path);
}

#endif

DEATH_API_EXPORT void Texture2D::release()
{
    m_impl->release();
}

DEATH_API_EXPORT void Texture2D::update(
            const arc::lx::Vector2u& offset,
            const arc::lx::Vector2u& size,
            GLenum format,
            GLenum data_type,
            const GLvoid* data)
{
    m_impl->update(offset, size, format, data_type, data);
}

DEATH_API_EXPORT void Texture2D::set_filtering(
        GLenum min_filter,
        GLenum mag_filter)
{
    m_impl->set_filtering(min_filter, mag_filter);
}

DEATH_API_EXPORT void Texture2D::set_wrap(GLenum wrap_s, GLenum wrap_t)
{
    m_impl->set_wrap(wrap_s, wrap_t);
}

DEATH_API_EXPORT void Texture2D::bind()
{
    m_impl->bind();
}

DEATH_API_EXPORT void Texture2D::unbind()
{
    m_impl->unbind();
}

DEATH_API_EXPORT GLuint Texture2D::get_native() const
{
    return m_impl->get_native();
}

} // namespace gl
} // namespace death
