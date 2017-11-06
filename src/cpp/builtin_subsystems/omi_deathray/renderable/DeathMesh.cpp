#include "omi_deathray/renderable/DeathMesh.hpp"

#include <memory>

#include <deathray/api/Geometric.h>
#include <deathray/api/Spatial.h>
#include <deathray/api/VBO.h>

// TODO: REMOVE ME
#include <deathray/Geometry.hpp>
#include <deathray/Renderer.hpp>

// TODO: REMOVE ME
#include <GL/glew.h>
#include <deathray/gl/ShaderProgram.hpp>


namespace omi_death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class DeathMesh::DeathMeshImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // The mesh component being handled
    omi::scene::Mesh* m_component;
    // The DeathRay scene object
    DeathSceneHandle m_scene;

    // spatial entity
    DeathSpatialHandle m_spatial;
    // geometric data
    DeathGeometricHandle m_geometric;
    // VBO for vertex positions
    DeathVBOHandle m_position_buffer;

    // TODO: REMOVE ME
    // the DeathRay geometric representation for this object
    std::unique_ptr<death::Geometry> m_geometry;

    // TODO: REMOVE ME
    GLuint m_vao;
    GLuint m_vertex_positons;
    std::size_t m_position_count;
    death::gl::ShaderProgram m_program;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    DeathMeshImpl(omi::scene::Mesh* component, DeathSceneHandle scene)
        : m_component      (component)
        , m_scene          (scene)
        , m_spatial        (nullptr)
        , m_geometric      (nullptr)
        , m_position_buffer(nullptr)
        , m_geometry       (nullptr)
        // TODO: REMOVE ME
        , m_vao            (0)
        , m_vertex_positons(0)
    {
        // generate vbos
        death_vbo_gen(1, &m_position_buffer);

        // pass positions to the VBO
        const std::vector<float>& positions =
            m_component->get_vertex_positions();
        death_vbo_set_data(
            m_position_buffer,
            kDeathFloat,
            static_cast<DeathSize>(positions.size()),
            3,
            &positions[0]
        );

        // generate the geometric
        death_geo_gen(1, &m_geometric);
        // attach vbos to geometric
        death_geo_attach_vbo(m_geometric, 0, m_position_buffer);

        // generate the spatial entity
        death_spatial_gen(1, &m_spatial);
        // attach geometric to the spatial
        death_spatial_attach_geo(m_spatial, m_geometric);

        // add the spatial to the scene
        death_scene_add_spatial(m_scene, m_spatial);

        //----------------------------------------------------------------------

        // // set deathray geometry
        // m_geometry.reset(
        //     new death::Geometry(m_component->get_vertex_positions())
        // );
        // // add to the renderer
        // death::Renderer::instance().add_geometry(m_geometry.get());

        // // TODO: sort out initialisation somewhere
        // arc::io::sys::Path vertex_path;
        // vertex_path
        //     << "res" << "deathray" << "gl" << "shaders" << "vertex"
        //     << "test.glsl";
        // arc::io::sys::Path fragment_path;
        // fragment_path
        //     << "res" << "deathray" << "gl" << "shaders" << "fragment"
        //     << "test.glsl";

        // m_program.attach_shader_from_file(GL_VERTEX_SHADER, vertex_path);
        // m_program.attach_shader_from_file(GL_FRAGMENT_SHADER, fragment_path);
        // m_program.link();

        // glGenVertexArrays(1, &m_vao);
        // glBindVertexArray(m_vao);

        // const std::vector<float>& vertex_positons =
        //     m_component->get_vertex_positions();
        // m_position_count = vertex_positons.size() / 3;


        // glGenBuffers(1, &m_vertex_positons);
        // glBindBuffer(GL_ARRAY_BUFFER, m_vertex_positons);
        // glBufferData(
        //     GL_ARRAY_BUFFER,
        //     vertex_positons.size() * sizeof(GLfloat),
        //     &vertex_positons[0],
        //     GL_STATIC_DRAW
        // );

        // glEnableVertexAttribArray(0);
        // glBindBuffer(GL_ARRAY_BUFFER, m_vertex_positons);
        // glVertexAttribPointer(
        //     0,                    // attribute 0
        //     3,                    // size
        //     GL_FLOAT,             // type
        //     GL_FALSE,             // normalized?
        //     0,                    // stride
        //     static_cast<void*>(0) // array buffer offset
        // );

        // glBindBuffer(GL_ARRAY_BUFFER, 0);
        // glBindVertexArray(0);
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~DeathMeshImpl()
    {
        // clean up
        death_spatial_delete(1, &m_spatial);
        death_geo_delete(1, &m_geometric);
        death_vbo_delete(1, &m_position_buffer);
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    void render(const arc::lx::Matrix44f& vp_matrix)
    {
        // glBindVertexArray(m_vao);
        // m_program.bind();
        // m_program.set_uniform_44f("u_vp_matrix", vp_matrix);
        // glDrawArrays(GL_TRIANGLES, 0, m_position_count);
        // m_program.unbind();
        // glBindVertexArray(0);

        // render debug bounds
        // m_geometry->draw_gl_bounds(vp_matrix);
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

DeathMesh::DeathMesh(omi::scene::Mesh* component, DeathSceneHandle scene)
    : m_impl(new DeathMeshImpl(component, scene))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

DeathMesh::~DeathMesh()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void DeathMesh::render(const arc::lx::Matrix44f& vp_matrix)
{
    m_impl->render(vp_matrix);
}

} // namespace omi_death
