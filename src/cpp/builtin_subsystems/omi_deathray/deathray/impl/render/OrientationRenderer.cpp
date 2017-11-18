/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/render/OrientationRenderer.hpp"

#include <arcanecore/lx/MatrixMath44f.hpp>

#include <GL/glew.h>

#include "deathray/gl/ErrorState.hpp"
#include "deathray/gl/ShaderProgram.hpp"

#include "deathray/impl/Camera.hpp"
#include "deathray/impl/Scene.hpp"


namespace death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class OrientationRenderer::OrientationRendererImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the OpenGL shader program used to render the orientation widget
    death::gl::ShaderProgram m_shader_program;
    // the vertex array object for the widget
    GLuint m_vao;
    // the number of points in the widget
    std::size_t m_number_of_points;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    OrientationRendererImpl()
        : m_vao             (0)
        , m_number_of_points(0)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~OrientationRendererImpl()
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
        death::gl::error::check_state("before OrientationRenderer");

        // perform setup once
        static_setup();

        // set state
        glDisable(GL_DEPTH_TEST);
        glLineWidth(2.0F);

        // get the rotation from the camera
        float yaw   = 0.0F;
        float pitch = 0.0F;
        float roll  = 0.0F;
        death::Camera* camera = scene->get_camera();
        arc::lx::extract_euler_from_affine_44f(
            camera->get_transform(),
            yaw,
            pitch,
            roll
        );

        arc::lx::Matrix44f mvp_matrix = arc::lx::Matrix44f::Identity();
        mvp_matrix *= camera->get_projection_matrix();
        mvp_matrix *= camera->get_transform().inverse();
        //     arc::lx::translate_44f(arc::lx::Vector3f(-0.8F, -0.8F, 0.0F));
        // // rotate
        // mvp_matrix *=
        //     arc::lx::rotate_euler_44f(arc::lx::Vector3f(roll, pitch, yaw));

        // bind
        glBindVertexArray(m_vao);
        m_shader_program.bind();

        // TODO: just extract rotation
        m_shader_program.set_uniform_44f("u_mvp_matrix", mvp_matrix);

        glDrawArrays(GL_LINES, 0, m_number_of_points);

        // unbind
        m_shader_program.unbind();
        glBindVertexArray(0);

        death::gl::error::check_state("after OrientationRenderer");
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
            << "orientation.glsl";
        arc::io::sys::Path fragment_path;
        fragment_path
            << "res" << "deathray" << "gl" << "shaders" << "fragment"
            << "orientation.glsl";

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
        static const float size = 0.15F;
        static const std::vector<float> positions = {
            0.0F, 0.0F, 0.0F,
            size, 0.0F, 0.0F,

            0.0F, 0.0F, 0.0F,
            0.0F, size, 0.0F,

            0.0F, 0.0F, 0.0F,
            0.0F, 0.0F, size
        };
        m_number_of_points = positions.size() / 3;
        // setuo colours
        static const std::vector<float> colours = {
            1.0F, 0.0F, 0.0F,
            1.0F, 0.0F, 0.0F,

            0.0F, 1.0F, 0.0F,
            0.0F, 1.0F, 0.0F,

            0.0F, 0.0F, 1.0F,
            0.0F, 0.0F, 1.0F
        };

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

        GLuint colour_buffer = 0;
        glGenBuffers(1, &colour_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, colour_buffer);
        glBufferData(
            GL_ARRAY_BUFFER,
            colours.size() * sizeof(float),
            &colours[0],
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
        glBindBuffer(GL_ARRAY_BUFFER, colour_buffer);
        glVertexAttribPointer(
            1,                    // attribute 1
            3,                    // size
            GL_FLOAT,             // type
            GL_FALSE,             // normalized?
            0,                    // stride
            static_cast<void*>(0) // array buffer offset
        );

        // clean up
        glBindVertexArray(0);
        glDeleteBuffers(1, &colour_buffer);
        glDeleteBuffers(1, &position_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

OrientationRenderer& OrientationRenderer::instance()
{
    static OrientationRenderer inst;
    return inst;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void OrientationRenderer::render(death::Scene* scene)
{
    m_impl->render(scene);
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

OrientationRenderer::OrientationRenderer()
    : m_impl(new OrientationRendererImpl())
{
}

//------------------------------------------------------------------------------
//                               PRIVATE DESTRUCTOR
//------------------------------------------------------------------------------

OrientationRenderer::~OrientationRenderer()
{
    delete m_impl;
}

} // namespace death
