#include "deathray/Renderer.hpp"

#include <list>

#include <GL/glew.h>

#include <deathray/gl/ShaderProgram.hpp>

#include "deathray/Geometry.hpp"
#include "deathray/TraceOctant.hpp"


namespace death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class Renderer::RendererImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    std::list<Geometry*> m_geometry;

    death::gl::ShaderProgram m_program;

    // screenspace quad gl data
    GLuint m_ssquad_vao;
    std::size_t m_ssquad_point_count;

    // bvh data
    std::size_t m_bvh_size;
    GLuint m_bvh_buffer;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    RendererImpl()
        : m_ssquad_vao        (0)
        , m_ssquad_point_count(0)
        , m_bvh_size          (0)
        , m_bvh_buffer        (0)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~RendererImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    void setup()
    {
        // compile shaders
        arc::io::sys::Path vertex_path;
        vertex_path
            << "res" << "deathray" << "gl" << "shaders" << "vertex"
            << "deathray_renderer.glsl";
        arc::io::sys::Path fragment_path;
        fragment_path
            << "res" << "deathray" << "gl" << "shaders" << "fragment"
            << "deathray_renderer.glsl";

        m_program.attach_shader_from_file(
            GL_VERTEX_SHADER,
            vertex_path
        );
        m_program.attach_shader_from_file(
            GL_FRAGMENT_SHADER,
            fragment_path
        );
        m_program.link();

        static const std::vector<float> positions = {
            -1.0f, -1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f
        };
        static const std::vector<float> uvs = {
            0.0F, 0.0F,
            1.0F, 0.0F,
            1.0F, 1.0F,
            1.0F, 1.0F,
            0.0F, 1.0F,
            0.0F, 0.0F
        };

        m_ssquad_point_count = positions.size() / 3;

        glGenVertexArrays(1, &m_ssquad_vao);
        glBindVertexArray(m_ssquad_vao);

        GLuint position_buffer = 0;
        glGenBuffers(1, &position_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
        glBufferData(
            GL_ARRAY_BUFFER,
            positions.size() * sizeof(float),
            &positions[0],
            GL_STATIC_DRAW
        );

        GLuint uv_buffer = 0;
        glGenBuffers(1, &uv_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(uvs) * sizeof(float),
            &uvs[0],
            GL_STATIC_DRAW
        );

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
        glVertexAttribPointer(
            0,                    // attribute 0
            3,                    // size
            GL_FLOAT,             // type
            GL_FALSE,             // normalized?
            0,                    // stride
            static_cast<void*>(0) // array buffer offset
        );
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
        glVertexAttribPointer(
            1,                    // attribute 1
            2,                    // size
            GL_FLOAT,             // type
            GL_FALSE,             // normalized?
            0,                    // stride
            static_cast<void*>(0) // array buffer offset
        );

        // clean up
        glBindVertexArray(0);
        glDeleteBuffers(1, &uv_buffer);
        glDeleteBuffers(1, &position_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void render()
    {
        glBindVertexArray(m_ssquad_vao);
        m_program.bind();

        glActiveTexture(GL_TEXTURE0);
        m_program.set_uniform_1u("u_bvh", 0);
        glBindTexture(GL_TEXTURE_2D, m_bvh_buffer);

        m_program.set_uniform_1u("u_bvh_size", m_bvh_size);

        glDrawArrays(GL_TRIANGLES, 0, m_ssquad_point_count);
        m_program.unbind();
        glBindVertexArray(0);
    }

    void add_geometry(Geometry* geo)
    {
        m_geometry.push_back(geo);

        // release any existing bvh data
        if(m_bvh_buffer != 0)
        {
            glDeleteTextures(1, &m_bvh_buffer);
        }

        // build the bvh
        glEnable(GL_TEXTURE_1D);
        glGenTextures(1, &m_bvh_buffer);
        glBindTexture(GL_TEXTURE_1D, m_bvh_buffer);

        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        const std::list<death::TraceOctant*>& octants = geo->get_octants();
        m_bvh_size = octants.size();

        std::vector<float> buffer;
        buffer.reserve(octants.size());
        for(const death::TraceOctant* octant : octants)
        {
            const arc::lx::Vector3f& p = octant->get_position();
            buffer.push_back(p(0));
            buffer.push_back(p(1));
            buffer.push_back(p(2));
        }

        glTexImage1D(
            GL_TEXTURE_1D,
            0,
            GL_RGBA32F,
            octants.size(),
            0,
            GL_RGB,
            GL_FLOAT,
            &buffer[0]
        );
    }
};

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

DEATH_API_EXPORT Renderer& Renderer::instance()
{
    static Renderer inst;
    return inst;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

DEATH_API_EXPORT void Renderer::setup()
{
    m_impl->setup();
}

DEATH_API_EXPORT void Renderer::render()
{
    m_impl->render();
}

DEATH_API_EXPORT void Renderer::add_geometry(Geometry* geo)
{
    m_impl->add_geometry(geo);
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

Renderer::Renderer()
    : m_impl(new RendererImpl())
{
}

//------------------------------------------------------------------------------
//                               PRIVATE DESTRUCTOR
//------------------------------------------------------------------------------

Renderer::~Renderer()
{
    delete m_impl;
}

} // namespace death
