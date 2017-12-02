/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/render/RayRenderer.hpp"

#include <arcanecore/lx/Vector.hpp>

#include <GL/glew.h>

#include "deathray/gl/ErrorState.hpp"
#include "deathray/gl/ShaderProgram.hpp"

#include "deathray/impl/Camera.hpp"
#include "deathray/impl/Scene.hpp"
#include "deathray/impl/pathtracer/cpu/CPULightPath.hpp"
#include "deathray/impl/pathtracer/cpu/CPURayHierarchy.hpp"


namespace death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class RayRenderer::RayRendererImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the OpenGL shader program used to render rays
    death::gl::ShaderProgram m_shader_program;

    // the vertex array object for the rays geometry
    GLuint m_vao;
    // the number of points in the rays geometry
    DeathSize m_number_of_points;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    RayRendererImpl()
        : m_vao             (0)
        , m_number_of_points(0)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~RayRendererImpl()
    {
        release_gl_data();
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    void render(death::Scene* scene)
    {
        death::gl::error::check_state("before OctreeRenderer");

        // perform setup once
        static_setup();

        // get the camera from the scene
        death::Camera* camera = scene->get_camera();

        // trace
        std::vector<death::cpu::RayHierarchy*> rays;

        // TODO: where to source which part of the image plane to trace?
        {
            arc::lx::Vector2f plane_position(0.0F, 0.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(-10.0F, -10.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(-10.0F, 10.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(10.0F, -10.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(10.0F, 10.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(-5.0F, -5.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(-5.0F, 5.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(5.0F, -5.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(5.0F, 5.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(-2.0F, -2.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(-2.0F, 2.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(2.0F, -2.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(2.0F, 2.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(-1.0F, -1.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(-1.0F, 1.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(1.0F, -1.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(1.0F, 1.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(-2.5F, -2.5F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(-2.5F, 2.5F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(2.5F, -2.5F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(2.5F, 2.5F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(-3.0F, -3.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(-3.0F, 3.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(3.0F, -3.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(3.0F, 3.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(-3.5F, -3.5F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(-3.5F, 3.5F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(3.5F, -3.5F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(3.5F, 3.5F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(-4.0F, -4.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(-4.0F, 4.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(4.0F, -4.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(4.0F, 4.0F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(-4.5F, -4.5F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(-4.5F, 4.5F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(4.5F, -4.5F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        {
            arc::lx::Vector2f plane_position(4.5F, 4.5F);
            death::cpu::LightPath path(
                camera->get_focal_length(),
                plane_position
            );
            death::cpu::RayHierarchy* ray_hierarchy =
                new death::cpu::RayHierarchy();
            path.trace(scene, ray_hierarchy, true);
            rays.push_back(ray_hierarchy);
        }

        // create GL data for the ray hierarchy
        create_gl_data(rays);

        // set state
        glDisable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glLineWidth(1.0F);

        // set up the shader program
        m_shader_program.bind();

        // set transform
        arc::lx::Matrix44f mvp_matrix = camera->get_projection_matrix();
        mvp_matrix *= camera->get_transform().inverse();
        m_shader_program.set_uniform_44f("u_mvp_matrix", mvp_matrix);

        glBindVertexArray(m_vao);
        glDrawArrays(GL_LINES, 0, m_number_of_points);
        glBindVertexArray(0);

        m_shader_program.unbind();

        death::gl::error::check_state("after OctreeRenderer");
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
            << "rays.glsl";
        arc::io::sys::Path fragment_path;
        fragment_path
            << "res" << "deathray" << "gl" << "shaders" << "fragment"
            << "rays.glsl";

        m_shader_program.attach_shader_from_file(
            GL_VERTEX_SHADER,
            vertex_path
        );
        m_shader_program.attach_shader_from_file(
            GL_FRAGMENT_SHADER,
            fragment_path
        );
        m_shader_program.link();
    }

    void create_gl_data(const std::vector<death::cpu::RayHierarchy*>& rays)
    {
        release_gl_data();

        // build the data
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        // create lines from each of the rays in the heirarchy
        std::vector<float> positions;
        for(const death::cpu::RayHierarchy* ray : rays)
        {
            recurse_rays(ray, positions);
        }

        m_number_of_points = positions.size() / 3;

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

    void recurse_rays(
            const death::cpu::RayHierarchy* rays,
            std::vector<float>& positions)
    {
        positions.push_back(rays->start(0));
        positions.push_back(rays->start(1));
        positions.push_back(rays->start(2));
        positions.push_back(rays->end(0));
        positions.push_back(rays->end(1));
        positions.push_back(rays->end(2));

        for(death::cpu::RayHierarchy* child : rays->children)
        {
            recurse_rays(child, positions);
        }
    }

    void release_gl_data()
    {
        m_number_of_points = 0;
        if(m_vao != 0)
        {
            glDeleteVertexArrays(1, &m_vao);
            m_vao = 0;
        }
    }
};

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

RayRenderer& RayRenderer::instance()
{
    static RayRenderer inst;
    return inst;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void RayRenderer::render(death::Scene* scene)
{
    m_impl->render(scene);
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

RayRenderer::RayRenderer()
    : m_impl(new RayRendererImpl())
{
}

//------------------------------------------------------------------------------
//                               PRIVATE DESTRUCTOR
//------------------------------------------------------------------------------

RayRenderer::~RayRenderer()
{
    delete m_impl;
}

} // namespace death
