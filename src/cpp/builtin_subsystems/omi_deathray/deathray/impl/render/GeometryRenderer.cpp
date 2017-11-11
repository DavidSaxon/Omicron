/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/render/GeometryRenderer.hpp"

#include <arcanecore/lx/Vector.hpp>

#include <GL/glew.h>

#include <deathray/gl/ShaderProgram.hpp>

#include "deathray/impl/Camera.hpp"
#include "deathray/impl/Geometric.hpp"
#include "deathray/impl/Octree.hpp"
#include "deathray/impl/Scene.hpp"
#include "deathray/impl/Spatial.hpp"
#include "deathray/impl/debug/GLGeometry.hpp"

// TODO: REMOVE ME
#include <iostream>


namespace death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class GeometryRenderer::GeometryRendererImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the OpenGL shader program used to render geometry
    death::gl::ShaderProgram m_shader_program;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    GeometryRendererImpl()
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~GeometryRendererImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    void render(death::Scene* scene)
    {
        // perform setup once
        static_setup();

        // set state
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);

        // set up the shader program
        m_shader_program.bind();

        death::Camera* camera = scene->get_camera();
        arc::lx::Matrix44f mvp_matrix = camera->get_projection_matrix();
        mvp_matrix *= camera->get_transform().inverse();
        m_shader_program.set_uniform_44f("u_mvp_matrix", mvp_matrix);

        // draw gl geometry
        for(death::Spatial* spatial : scene->get_spatials())
        {
            // calculate the size of the geometry
            const arc::lx::AABB3f& true_bounds =
                scene->get_or_create_octree(spatial)->get_true_bounds();
            arc::lx::Vector3f geo_size(
                std::abs(true_bounds.min()(0)),
                std::abs(true_bounds.min()(1)),
                std::abs(true_bounds.min()(2))
            );
            if(geo_size(0) < std::abs(true_bounds.max()(0)))
            {
                geo_size(0) = std::abs(true_bounds.max()(0));
            }
            if(geo_size(1) < std::abs(true_bounds.max()(1)))
            {
                geo_size(1) = std::abs(true_bounds.max()(1));
            }
            if(geo_size(2) < std::abs(true_bounds.max()(2)))
            {
                geo_size(2) = std::abs(true_bounds.max()(2));
            }
            // pass to the shader
            m_shader_program.set_uniform_3f("u_size", geo_size);

            for(death::Geometric* geometric : spatial->get_geometrics())
            {
                death::GLGeometry* geo = geometric->get_debug_geo();
                if(geo != nullptr)
                {
                    geo->draw();
                }
            }
        }

        m_shader_program.unbind();
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
            << "debug_geometry.glsl";
        arc::io::sys::Path fragment_path;
        fragment_path
            << "res" << "deathray" << "gl" << "shaders" << "fragment"
            << "debug_geometry.glsl";

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
};

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

GeometryRenderer& GeometryRenderer::instance()
{
    static GeometryRenderer inst;
    return inst;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void GeometryRenderer::render(death::Scene* scene)
{
    m_impl->render(scene);
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

GeometryRenderer::GeometryRenderer()
    : m_impl(new GeometryRendererImpl())
{
}

//------------------------------------------------------------------------------
//                               PRIVATE DESTRUCTOR
//------------------------------------------------------------------------------

GeometryRenderer::~GeometryRenderer()
{
    delete m_impl;
}

} // namespace death
