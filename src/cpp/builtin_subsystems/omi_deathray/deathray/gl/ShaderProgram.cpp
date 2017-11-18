#include "deathray/gl/ShaderProgram.hpp"

#include <list>

#include <arcanecore/base/Exceptions.hpp>
#include <arcanecore/io/sys/FileReader.hpp>

#include "deathray/gl/ErrorState.hpp"
#include "deathray/impl/Globals.hpp"


namespace death
{
namespace gl
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class ShaderProgram::ShaderProgramImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the id of the shader program
    GLuint m_program_id;

    // the currently compiled shaders to link
    std::list<GLuint> m_compiled_shaders;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    ShaderProgramImpl()
        : m_program_id(0)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    virtual ~ShaderProgramImpl()
    {
        release();
    }

    //-------------P U B L I C    S T A T I C    F U N C T I O N S--------------

    static arc::str::UTF8String shader_type_to_string(GLenum shader_type)
    {
        switch(shader_type)
        {
            case GL_COMPUTE_SHADER:
                return "compute";
            case GL_VERTEX_SHADER:
                return "vertex";
            case GL_TESS_CONTROL_SHADER:
                return "tessellation control";
            case GL_TESS_EVALUATION_SHADER:
                return "tessellation evaluation";
            case GL_GEOMETRY_SHADER:
                return "geometry";
            case GL_FRAGMENT_SHADER:
                return "fragment";
            default:
                return "UNKNOWN";
        }
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    void attach_shader_from_string(
            GLenum shader_type,
            const arc::str::UTF8String& source)
    {
        GLuint shader_id = glCreateShader(shader_type);

        // compile the shader
        const char* shader_source = source.get_raw();
        glShaderSource(shader_id, 1, &shader_source, nullptr);
        glCompileShader(shader_id);

        // check for errors
        GLint compile_result = GL_FALSE;
        int error_length = 0;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compile_result);
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &error_length);
        if(error_length > 0)
        {
            std::vector<char> gl_error_message(error_length+1);
            glGetShaderInfoLog(
                shader_id,
                error_length,
                nullptr,
                &gl_error_message[0]
            );

            DEATH_LOG_ERROR(
                "Failed to compile " << shader_type_to_string(shader_type)
                << " shader with error: " << (&gl_error_message[0])
            );
            return;
        }

        m_compiled_shaders.push_back(shader_id);
    }

    void attach_shader_from_file(
            GLenum shader_type,
            const arc::io::sys::Path& source)
    {
        arc::io::sys::FileReader reader(source);

        arc::str::UTF8String contents;
        reader.read(contents);
        reader.close();

        attach_shader_from_string(shader_type, contents);
    }

    void link()
    {
        release();

        m_program_id = glCreateProgram();
        // attach shaders
        for(GLuint shader_id : m_compiled_shaders)
        {
            glAttachShader(m_program_id, shader_id);
        }
        // link
        glLinkProgram(m_program_id);

        // check for errors
        GLint link_result = GL_FALSE;
        int error_length = 0;
        glGetProgramiv(m_program_id, GL_LINK_STATUS, &link_result);
        glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &error_length);
        if(error_length > 0)
        {
            std::vector<char> gl_error_message(error_length+1);
            glGetProgramInfoLog(
                m_program_id,
                error_length,
                nullptr,
                &gl_error_message[0]
            );

            DEATH_LOG_ERROR(
                "Failed to link shader program with error: "
                << (&gl_error_message[0])
            );
            return;
        }
    }

    void release()
    {
        if(m_program_id != 0)
        {
            glDeleteProgram(m_program_id);
            m_program_id = 0;
        }
    }

    void bind()
    {
        glUseProgram(m_program_id);
    }

    void unbind()
    {
        glUseProgram(0);
    }

    GLuint get_native() const
    {
        return m_program_id;
    }

    GLint get_uniform_location(const arc::str::UTF8String& location)
    {
        return glGetUniformLocation(m_program_id, location.get_raw());
    }

    void set_uniform_1i(
            const arc::str::UTF8String& location,
            arc::int32 value)
    {
        GLint location_id = get_uniform_location(location);
        if(location_id >= 0)
        {
            glUniform1i(location_id, value);
        }
    }

    void set_uniform_1u(
            const arc::str::UTF8String& location,
            arc::uint32 value)
    {
        GLint location_id = get_uniform_location(location);
        if(location_id >= 0)
        {
            glUniform1ui(location_id, value);
        }
    }

    void set_uniform_1f(
            const arc::str::UTF8String& location,
            float value)
    {
        GLint location_id = get_uniform_location(location);
        if(location_id >= 0)
        {
            glUniform1f(location_id, value);
        }
    }

    void set_uniform_3f(
            const arc::str::UTF8String& location,
            const arc::lx::Vector3f& value)
    {
        GLint location_id = get_uniform_location(location);
        if(location_id >= 0)
        {
            glUniform3f(location_id, value(0), value(1), value(2));
        }
    }

    void set_uniform_44f(
            const arc::str::UTF8String& location,
            const arc::lx::Matrix44f& value)
    {
        GLint location_id = get_uniform_location(location);
        if(location_id >= 0)
        {
            glUniformMatrix4fv(location_id, 1, GL_FALSE, &value(0, 0));
        }
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

ShaderProgram::ShaderProgram()
    : m_impl(new ShaderProgramImpl())
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

DEATH_API_EXPORT ShaderProgram::~ShaderProgram()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

DEATH_API_EXPORT arc::str::UTF8String ShaderProgram::shader_type_to_string(
            GLenum shader_type)
{
    return ShaderProgramImpl::shader_type_to_string(shader_type);
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

DEATH_API_EXPORT void ShaderProgram::attach_shader_from_string(
        GLenum shader_type,
        const arc::str::UTF8String& source)
{
    m_impl->attach_shader_from_string(shader_type, source);
}

DEATH_API_EXPORT void ShaderProgram::attach_shader_from_file(
        GLenum shader_type,
        const arc::io::sys::Path& source)
{
    m_impl->attach_shader_from_file(shader_type, source);
}

DEATH_API_EXPORT void ShaderProgram::link()
{
    m_impl->link();
}

DEATH_API_EXPORT void ShaderProgram::release()
{
    m_impl->release();
}

DEATH_API_EXPORT void ShaderProgram::bind()
{
    m_impl->bind();
}

DEATH_API_EXPORT void ShaderProgram::unbind()
{
    m_impl->unbind();
}

DEATH_API_EXPORT GLuint ShaderProgram::get_native() const
{
    return m_impl->get_native();
}

DEATH_API_EXPORT GLint ShaderProgram::get_uniform_location(
        const arc::str::UTF8String& location)
{
    return m_impl->get_uniform_location(location);
}

DEATH_API_EXPORT void ShaderProgram::set_uniform_1i(
        const arc::str::UTF8String& location,
        arc::int32 value)
{
    m_impl->set_uniform_1i(location, value);
}

DEATH_API_EXPORT void ShaderProgram::set_uniform_1u(
        const arc::str::UTF8String& location,
        arc::uint32 value)
{
    m_impl->set_uniform_1u(location, value);
}

DEATH_API_EXPORT void ShaderProgram::set_uniform_1f(
        const arc::str::UTF8String& location,
        float value)
{
    m_impl->set_uniform_1f(location, value);
}

DEATH_API_EXPORT void ShaderProgram::set_uniform_3f(
        const arc::str::UTF8String& location,
        const arc::lx::Vector3f& value)
{
    m_impl->set_uniform_3f(location, value);
}

DEATH_API_EXPORT void ShaderProgram::set_uniform_44f(
        const arc::str::UTF8String& location,
        const arc::lx::Matrix44f& value)
{
    m_impl->set_uniform_44f(location, value);
}

} // namespace gl
} // namespace death
