/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/render/BoundRenderer.hpp"

#include <GL/glew.h>

#include "deathray/gl/ErrorState.hpp"
#include "deathray/gl/ShaderProgram.hpp"

#include "deathray/impl/Camera.hpp"
#include "deathray/impl/Octree.hpp"
#include "deathray/impl/Scene.hpp"
#include "deathray/impl/Spatial.hpp"


namespace death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class BoundRenderer::BoundRendererImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the OpenGL shader program used to render bounds
    death::gl::ShaderProgram m_shader_program;
    // the shared vertex array object to use to render bounds
    GLuint m_vao;
    // the number of points in bound geometry
    std::size_t m_number_of_points;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    BoundRendererImpl()
        : m_vao             (0)
        , m_number_of_points(0)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~BoundRendererImpl()
    {
        // release bound geometry
        m_number_of_points = 0;
        if(m_vao != 0)
        {
            glDeleteVertexArrays(1, &m_vao);
            m_vao = 0;
        }
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    void render(death::Scene* scene)
    {
        death::gl::error::check_state("before BoundRenderer");

        // perform setup once
        static_setup();

        // set state
        glEnable(GL_DEPTH_TEST);
        glLineWidth(1.0F);

        // get camera data
        death::Camera* camera = scene->get_camera();
        arc::lx::Matrix44f mvp_matrix = camera->get_projection_matrix();
        mvp_matrix *= camera->get_transform().inverse();

        // bind
        glBindVertexArray(m_vao);
        m_shader_program.bind();

        // iterate over the spatial in the scene
        for(death::Spatial* spatial : scene->get_spatials())
        {
            // get the bounds to render
            const arc::lx::AABB3f& true_bounds =
                scene->get_or_create_octree(spatial)->get_true_bounds();
            // pass to the shader
            m_shader_program.set_uniform_3f("u_bound_min", true_bounds.min());
            m_shader_program.set_uniform_3f("u_bound_max", true_bounds.max());

            // TODO: get model matrix
            m_shader_program.set_uniform_44f("u_mvp_matrix", mvp_matrix);

            glDrawArrays(GL_LINES, 0, m_number_of_points);
        };

        // unbind
        m_shader_program.unbind();
        glBindVertexArray(0);

        death::gl::error::check_state("after BoundRenderer");
    }

private:

    //------------P R I V A T E    M E M B E R    F U N C T I O N S-------------

    void static_setup()
    {
        // only perform this once
        static bool do_once = true;
        if(!do_once)
        {
            return;
        }
        do_once = false;

        // setup the shader
        // TODO: shaders should be built into the resource code
        arc::io::sys::Path vertex_path;
        vertex_path
            << "res" << "deathray" << "gl" << "shaders" << "vertex"
            << "bound.glsl";
        arc::io::sys::Path fragment_path;
        fragment_path
            << "res" << "deathray" << "gl" << "shaders" << "fragment"
            << "bound.glsl";

        m_shader_program.attach_shader_from_file(
            GL_VERTEX_SHADER,
            vertex_path
        );
        m_shader_program.attach_shader_from_file(
            GL_FRAGMENT_SHADER,
            fragment_path
        );
        m_shader_program.link();

        // setup positions
        static const std::vector<float> positions = {
            -1.0F, -1.0F, -1.0F,
             1.0F, -1.0F, -1.0F,

             1.0F, -1.0F, -1.0F,
             1.0F,  1.0F, -1.0F,

             1.0F,  1.0F, -1.0F,
            -1.0F,  1.0F, -1.0F,

            -1.0F,  1.0F, -1.0F,
            -1.0F, -1.0F, -1.0F,

            -1.0F, -1.0F,  1.0F,
             1.0F, -1.0F,  1.0F,

             1.0F, -1.0F,  1.0F,
             1.0F,  1.0F,  1.0F,

             1.0F,  1.0F,  1.0F,
            -1.0F,  1.0F,  1.0F,

            -1.0F,  1.0F,  1.0F,
            -1.0F, -1.0F,  1.0F,

            -1.0F, -1.0F, -1.0F,
            -1.0F, -1.0F,  1.0F,

             1.0F, -1.0F, -1.0F,
             1.0F, -1.0F,  1.0F,

            -1.0F,  1.0F, -1.0F,
            -1.0F,  1.0F,  1.0F,

             1.0F,  1.0F, -1.0F,
             1.0F,  1.0F,  1.0F
        };
        m_number_of_points = positions.size() / 3;

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        GLuint position_buffer = 0;
        glGenBuffers(1, &position_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
        glBufferData(
            GL_ARRAY_BUFFER,
            positions.size() * sizeof(float),
            &positions[0],
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

        // clean up
        glBindVertexArray(0);
        glDeleteBuffers(1, &position_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

BoundRenderer& BoundRenderer::instance()
{
    static BoundRenderer inst;
    return inst;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void BoundRenderer::render(death::Scene* scene)
{
    m_impl->render(scene);
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

BoundRenderer::BoundRenderer()
    : m_impl(new BoundRendererImpl())
{
}

//------------------------------------------------------------------------------
//                               PRIVATE DESTRUCTOR
//------------------------------------------------------------------------------

BoundRenderer::~BoundRenderer()
{
    delete m_impl;
}

} // namespace death
