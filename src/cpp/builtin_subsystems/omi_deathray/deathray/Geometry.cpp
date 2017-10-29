#include "deathray/Geometry.hpp"

#include <limits>

#include <arcanecore/lx/AABB.hpp>
#include <arcanecore/lx/Alignment.hpp>
#include <arcanecore/lx/Vector.hpp>

#include <GL/glew.h>

#include <deathray/gl/ShaderProgram.hpp>

#include "deathray/TraceOctant.hpp"


namespace death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class Geometry::GeometryImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the axis aligned bounds of the geometry
    arc::lx::AABB3f m_bounds;
    // the size of this geometry
    arc::lx::Vector3f m_size;
    // the centre point of this geometry
    arc::lx::Vector3f m_origin;

    // TODO: make a sub class for this
    // OpenGL data for rendering bounds
    death::gl::ShaderProgram m_gl_bounds_program;
    GLuint m_gl_bounds_vao;
    std::size_t m_gl_bounds_point_count;

    // TODO: testing
    std::list<TraceOctant*> m_octants;

public:

    ARC_LX_ALIGNED_NEW

    //--------------------------C O N S T R U C T O R---------------------------

    GeometryImpl(const std::vector<float>& faces)
        : m_gl_bounds_vao        (0)
        , m_gl_bounds_point_count(0)
    {
        prefabricate(faces);
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~GeometryImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    const std::list<TraceOctant*>& get_octants() const
    {
        return m_octants;
    }

    void draw_gl_bounds(const arc::lx::Matrix44f& vp_matrix)
    {
        glLineWidth(3.0F);
        glBindVertexArray(m_gl_bounds_vao);
        m_gl_bounds_program.bind();
        m_gl_bounds_program.set_uniform_44f("u_vp_matrix", vp_matrix);
        glDrawArrays(GL_LINES, 0, m_gl_bounds_point_count);
        m_gl_bounds_program.unbind();
        glBindVertexArray(0);

        for(TraceOctant* octant : m_octants)
        {
            octant->draw_gl_debug(vp_matrix);
        }
    }

private:

    //------------P R I V A T E    M E M B E R    F U N C T I O N S-------------

    // TODO: DOC
    void prefabricate(const std::vector<float>& faces)
    {
        // reset bounds
        // m_bounds.min() = arc::lx::Vector3f(
        //     std::numeric_limits<float>::infinity(),
        //     std::numeric_limits<float>::infinity(),
        //     std::numeric_limits<float>::infinity()
        // );
        // m_bounds.max() = arc::lx::Vector3f(
        //     -std::numeric_limits<float>::infinity(),
        //     -std::numeric_limits<float>::infinity(),
        //     -std::numeric_limits<float>::infinity()
        // );
        // reset size and origin
        m_size = arc::lx::Vector3f(0.0F, 0.0F, 0.0F);
        m_origin = arc::lx::Vector3f(0.0F, 0.0F, 0.0F);

        // check faces
        // TODO: % 3

        std::size_t point_count = faces.size() / 3;
        const arc::lx::Vector3f* v =
            reinterpret_cast<const arc::lx::Vector3f*>(&faces[0]);

        // find bounds
        for(const arc::lx::Vector3f* i = v; i != (v + point_count); ++i)
        {
            m_bounds.extend(*i);
        }

        // set size
        m_size(0) = m_bounds.max()(0) - m_bounds.min()(0);
        m_size(1) = m_bounds.max()(1) - m_bounds.min()(1);
        m_size(2) = m_bounds.max()(2) - m_bounds.min()(2);
        // set origin
        m_origin(0) = m_bounds.min()(0) + (m_size(0) / 2.0F);
        m_origin(1) = m_bounds.min()(1) + (m_size(1) / 2.0F);
        m_origin(2) = m_bounds.min()(2) + (m_size(2) / 2.0F);

        // TODO: only do if requested
        generate_gl_bounds();

        // TODO: retain face data

        static const float cell_size = 0.01F;
        static const float half_cell = cell_size / 2.0F;


        // TODO: TESTING
        for(int x = 0; x < static_cast<int>(m_size(0) / cell_size); ++x)
        {
            for(int y = 0; y < static_cast<int>(m_size(1) / cell_size); ++y)
            {
                for(int z = 0; z < static_cast<int>(m_size(2) / cell_size); ++z)
                {
                    //  does this cell contain geometry?
                    arc::lx::AABB3f cell_bounds(
                        arc::lx::Vector3f(
                            m_bounds.min()(0) + (x * cell_size),
                            m_bounds.min()(1) + (y * cell_size),
                            m_bounds.min()(2) + (z * cell_size)
                        ),
                        arc::lx::Vector3f(
                            m_bounds.min()(0) + ((x + 1) * cell_size),
                            m_bounds.min()(1) + ((y + 1) * cell_size),
                            m_bounds.min()(2) + ((z + 1) * cell_size)
                        )
                    );

                    bool keep = false;
                    for(const arc::lx::Vector3f* p = v;
                        p != (v + point_count);
                        ++p)
                    {
                        if((*p)(0) > cell_bounds.min()(0) &&
                           (*p)(0) < cell_bounds.max()(0) &&
                           (*p)(1) > cell_bounds.min()(1) &&
                           (*p)(1) < cell_bounds.max()(1) &&
                           (*p)(2) > cell_bounds.min()(2) &&
                           (*p)(2) < cell_bounds.max()(2))
                        {
                            keep = true;
                            break;
                        }
                    }
                    if(!keep)
                    {
                        continue;
                    }

                    m_octants.push_back(new TraceOctant(
                        arc::lx::Vector3f(
                            cell_bounds.min()(0) + half_cell,
                            cell_bounds.min()(1) + half_cell,
                            cell_bounds.min()(2) + half_cell
                        ),
                        cell_size
                    ));
                }
            }
        }
    }

    // generate OpenGL renderable geometry for bounds
    void generate_gl_bounds()
    {
        release_gl_bounds();

        // compile shaders
        arc::io::sys::Path vertex_path;
        vertex_path
            << "res" << "deathray" << "gl" << "shaders" << "vertex"
            << "proto_bound.glsl";
        arc::io::sys::Path fragment_path;
        fragment_path
            << "res" << "deathray" << "gl" << "shaders" << "fragment"
            << "proto_bound.glsl";

        m_gl_bounds_program.attach_shader_from_file(
            GL_VERTEX_SHADER,
            vertex_path
        );
        m_gl_bounds_program.attach_shader_from_file(
            GL_FRAGMENT_SHADER,
            fragment_path
        );
        m_gl_bounds_program.link();

        // build positions
        std::vector<float> positions = {
            m_bounds.min()(0), m_bounds.min()(1), m_bounds.min()(2),
            m_bounds.max()(0), m_bounds.min()(1), m_bounds.min()(2),

            m_bounds.max()(0), m_bounds.min()(1), m_bounds.min()(2),
            m_bounds.max()(0), m_bounds.max()(1), m_bounds.min()(2),

            m_bounds.max()(0), m_bounds.max()(1), m_bounds.min()(2),
            m_bounds.min()(0), m_bounds.max()(1), m_bounds.min()(2),

            m_bounds.min()(0), m_bounds.max()(1), m_bounds.min()(2),
            m_bounds.min()(0), m_bounds.min()(1), m_bounds.min()(2),

            m_bounds.min()(0), m_bounds.min()(1), m_bounds.max()(2),
            m_bounds.max()(0), m_bounds.min()(1), m_bounds.max()(2),

            m_bounds.max()(0), m_bounds.min()(1), m_bounds.max()(2),
            m_bounds.max()(0), m_bounds.max()(1), m_bounds.max()(2),

            m_bounds.max()(0), m_bounds.max()(1), m_bounds.max()(2),
            m_bounds.min()(0), m_bounds.max()(1), m_bounds.max()(2),

            m_bounds.min()(0), m_bounds.max()(1), m_bounds.max()(2),
            m_bounds.min()(0), m_bounds.min()(1), m_bounds.max()(2),

            m_bounds.min()(0), m_bounds.min()(1), m_bounds.min()(2),
            m_bounds.min()(0), m_bounds.min()(1), m_bounds.max()(2),

            m_bounds.max()(0), m_bounds.min()(1), m_bounds.min()(2),
            m_bounds.max()(0), m_bounds.min()(1), m_bounds.max()(2),

            m_bounds.min()(0), m_bounds.max()(1), m_bounds.min()(2),
            m_bounds.min()(0), m_bounds.max()(1), m_bounds.max()(2),

            m_bounds.max()(0), m_bounds.max()(1), m_bounds.min()(2),
            m_bounds.max()(0), m_bounds.max()(1), m_bounds.max()(2)
        };
        m_gl_bounds_point_count = positions.size() / 3;

        glGenVertexArrays(1, &m_gl_bounds_vao);
        glBindVertexArray(m_gl_bounds_vao);

        GLuint gl_bounds_positions = 0;
        glGenBuffers(1, &gl_bounds_positions);
        glBindBuffer(GL_ARRAY_BUFFER, gl_bounds_positions);
        glBufferData(
            GL_ARRAY_BUFFER,
            positions.size() * sizeof(float),
            &positions[0],
            GL_STATIC_DRAW
        );

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, gl_bounds_positions);
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
        glDeleteBuffers(1, &gl_bounds_positions);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // releases OpenGL renderable geometry for bounds
    void release_gl_bounds()
    {
        m_gl_bounds_point_count = 0;
        if(m_gl_bounds_vao != 0)
        {
            glDeleteVertexArrays(1, &m_gl_bounds_vao);
            m_gl_bounds_vao = 0;
        }
        m_gl_bounds_program.release();
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

DEATH_API_EXPORT Geometry::Geometry(const std::vector<float>& faces)
    : m_impl(new GeometryImpl(faces))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

DEATH_API_EXPORT Geometry::~Geometry()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

DEATH_API_EXPORT const std::list<TraceOctant*>& Geometry::get_octants() const
{
    return m_impl->get_octants();
}

DEATH_API_EXPORT void Geometry::draw_gl_bounds(
        const arc::lx::Matrix44f& vp_matrix)
{
    m_impl->draw_gl_bounds(vp_matrix);
}

} // namespace death
