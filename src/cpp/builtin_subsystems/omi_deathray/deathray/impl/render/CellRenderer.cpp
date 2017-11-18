/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/render/CellRenderer.hpp"

#include <arcanecore/lx/Vector.hpp>

#include <GL/glew.h>

#include "deathray/gl/ErrorState.hpp"
#include "deathray/gl/ShaderProgram.hpp"

#include "deathray/impl/Camera.hpp"
#include "deathray/impl/Octree.hpp"
#include "deathray/impl/Scene.hpp"
#include "deathray/impl/Spatial.hpp"
#include "deathray/impl/debug/GLCells.hpp"


namespace death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class CellRenderer::CellRendererImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the OpenGL shader program used to render octrees
    death::gl::ShaderProgram m_shader_program;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    CellRendererImpl()
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~CellRendererImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    void render(death::Scene* scene)
    {
        death::gl::error::check_state("before CellRenderer");

        // perform setup once
        static_setup();

        // set state
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        glLineWidth(1.0F);

        // set up the shader program
        m_shader_program.bind();

        death::Camera* camera = scene->get_camera();
        arc::lx::Matrix44f mvp_matrix = camera->get_projection_matrix();
        mvp_matrix *= camera->get_transform().inverse();

        // draw gl geometry
        for(death::Spatial* spatial : scene->get_spatials())
        {
            // get the octree
            death::Octree* octree = scene->get_or_create_octree(spatial);
            // get the gl octree
            death::GLCells* gl_cells = octree->get_debug_cells();

            if(gl_cells == nullptr)
            {
                continue;
            }

            // set transform
            m_shader_program.set_uniform_44f(
                "u_mvp_matrix",
                mvp_matrix * octree->get_offset()
            );

            gl_cells->draw();
        }

        m_shader_program.unbind();

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
            << "debug_cell.glsl";
        arc::io::sys::Path fragment_path;
        fragment_path
            << "res" << "deathray" << "gl" << "shaders" << "fragment"
            << "debug_cell.glsl";

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

CellRenderer& CellRenderer::instance()
{
    static CellRenderer inst;
    return inst;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void CellRenderer::render(death::Scene* scene)
{
    m_impl->render(scene);
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

CellRenderer::CellRenderer()
    : m_impl(new CellRendererImpl())
{
}

//------------------------------------------------------------------------------
//                               PRIVATE DESTRUCTOR
//------------------------------------------------------------------------------

CellRenderer::~CellRenderer()
{
    delete m_impl;
}

} // namespace death
