#include "deathray/TraceOctant.hpp"

#include <vector>

#include <arcanecore/crypt/hash/FNV.hpp>
#include <arcanecore/lx/MatrixMath44f.hpp>


namespace death
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

TraceOctant::TraceOctant(const arc::lx::Vector3f& position, float size)
    : m_position      (position)
    , m_size          (size)
    , m_gl_vao        (0)
    , m_gl_point_count(0)
{
    // TODO: this should be more robust
    // generate the id based on a hash of the position
    m_id.integer =
        arc::crypt::hash::fnv1a_32(&m_position, sizeof(arc::lx::Vector3f));
    m_gl_colour(0) = static_cast<float>(m_id.bytes[0]) / 255.0F;
    m_gl_colour(1) = static_cast<float>(m_id.bytes[1]) / 255.0F;
    m_gl_colour(2) = static_cast<float>(m_id.bytes[2]) / 255.0F;

    // compile shaders
    arc::io::sys::Path vertex_path;
    vertex_path
        << "res" << "deathray" << "gl" << "shaders" << "vertex"
        << "proto_trace_octant.glsl";
    arc::io::sys::Path fragment_path;
    fragment_path
        << "res" << "deathray" << "gl" << "shaders" << "fragment"
        << "proto_trace_octant.glsl";

    m_gl_program.attach_shader_from_file(
        GL_VERTEX_SHADER,
        vertex_path
    );
    m_gl_program.attach_shader_from_file(
        GL_FRAGMENT_SHADER,
        fragment_path
    );
    m_gl_program.link();

    static const std::vector<float> positions = {
        -0.5f, -0.5f, 0.5f,
         0.5f, -0.5f, 0.5f,
         0.5f,  0.5f, 0.5f,
         0.5f,  0.5f, 0.5f,
        -0.5f,  0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        0.5F, -0.5f, -0.5f,
        0.5F,  0.5f, -0.5f,
        0.5F,  0.5f,  0.5f,
        0.5F,  0.5f,  0.5f,
        0.5F, -0.5f,  0.5f,
        0.5F, -0.5f, -0.5f,

         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,

        -0.5F,  0.5f,  0.5f,
        -0.5F,  0.5f, -0.5f,
        -0.5F, -0.5f, -0.5f,
        -0.5F, -0.5f, -0.5f,
        -0.5F, -0.5f,  0.5f,
        -0.5F,  0.5f,  0.5f,

         0.5f, 0.5F,  0.5f,
         0.5f, 0.5F, -0.5f,
        -0.5f, 0.5F, -0.5f,
        -0.5f, 0.5F, -0.5f,
        -0.5f, 0.5F,  0.5f,
         0.5f, 0.5F,  0.5f,

        -0.5f, -0.5F, -0.5f,
         0.5f, -0.5F, -0.5f,
         0.5f, -0.5F,  0.5f,
         0.5f, -0.5F,  0.5f,
        -0.5f, -0.5F,  0.5f,
        -0.5f, -0.5F, -0.5f,
    };
    m_gl_point_count = positions.size() / 3;


    glGenVertexArrays(1, &m_gl_vao);
    glBindVertexArray(m_gl_vao);

    GLuint gl_positions = 0;
    glGenBuffers(1, &gl_positions);
    glBindBuffer(GL_ARRAY_BUFFER, gl_positions);
    glBufferData(
        GL_ARRAY_BUFFER,
        positions.size() * sizeof(float),
        &positions[0],
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, gl_positions);
    glVertexAttribPointer(
        0,                    // attribute 0
        3,                    // size
        GL_FLOAT,             // type
        GL_FALSE,             // normalized?
        0,                    // stride
        static_cast<void*>(0) // array buffer offset
    );

    // clean up
    glBindVertexArray(0);
    glDeleteBuffers(1, &gl_positions);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

TraceOctant::~TraceOctant()
{
    m_gl_point_count = 0;
    if(m_gl_vao != 0)
    {
        glDeleteVertexArrays(1, &m_gl_vao);
        m_gl_vao = 0;
    }
    m_gl_program.release();
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

const arc::lx::Vector3f& TraceOctant::get_position() const
{
    return m_position;
}

void TraceOctant::draw_gl_debug(const arc::lx::Matrix44f& vp_matrix)
{
    // compute the model matrix
    arc::lx::Matrix44f model_matrix =
        arc::lx::translate_44f(m_position) * arc::lx::scale_44f(m_size);

    glBindVertexArray(m_gl_vao);
    m_gl_program.bind();
    m_gl_program.set_uniform_44f("u_mvp_matrix", vp_matrix * model_matrix);
    m_gl_program.set_uniform_3f("u_colour", m_gl_colour);
    glDrawArrays(GL_TRIANGLES, 0, m_gl_point_count);
    m_gl_program.unbind();
    glBindVertexArray(0);

}

} // namespace death
