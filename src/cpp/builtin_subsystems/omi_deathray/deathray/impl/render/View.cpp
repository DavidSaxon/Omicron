/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/render/View.hpp"

#include <memory>

#include <arcanecore/lx/Alignment.hpp>

#include <GL/glew.h>

#include <deathray/gl/Framebuffer.hpp>
#include <deathray/gl/ShaderProgram.hpp>
#include <deathray/gl/Texture2D.hpp>

#include "deathray/impl/Scene.hpp"
#include "deathray/impl/render/BoundRenderer.hpp"
#include "deathray/impl/render/CellRenderer.hpp"
#include "deathray/impl/render/GeometryRenderer.hpp"
#include "deathray/impl/render/OrientationRenderer.hpp"
#include "deathray/impl/render/PathTracer.hpp"
#include "deathray/impl/render/OctreeRenderer.hpp"


namespace death
{

//------------------------------------------------------------------------------
//                                    GLOBALS
//------------------------------------------------------------------------------

namespace
{

static std::unique_ptr<death::gl::ShaderProgram> g_shader_program(nullptr);

} // namespace anonymous


//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class View::ViewImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    arc::lx::AABB2f m_viewport;
    RenderMode m_render_modes;
    arc::lx::Vector3f m_background_colour;

    // the current resolution of this view
    arc::lx::Vector2u m_resolution;

    // denotes whether this view's surface needs rebuilding or not
    bool m_rebuild_surface;
    // the OpenGL Vertex Array Object to use to render the view's surface
    GLuint m_vao;
    // the number of points in the view's surface
    std::size_t m_number_of_points;

    // the framebuffer (and the texture components) this view renders to
    death::gl::Framebuffer m_framebuffer;
    death::gl::Texture2D m_colour_component;
    death::gl::Texture2D m_depth_component;

public:

    ARC_LX_ALIGNED_NEW;

    //--------------------------C O N S T R U C T O R---------------------------

    ViewImpl(
            const arc::lx::AABB2f& viewport,
            RenderMode render_modes,
            const arc::lx::Vector3f background_colour)
        : m_render_modes     (render_modes)
        , m_background_colour(background_colour)
        , m_resolution       (0, 0)
        , m_rebuild_surface  (true)
        , m_vao              (0)
        , m_number_of_points (0)
    {
        set_viewport(viewport);
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~ViewImpl()
    {
        // release surface geometry
        m_number_of_points = 0;
        if(m_vao != 0)
        {
            glDeleteVertexArrays(1, &m_vao);
            m_vao = 0;
        }
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    const arc::lx::AABB2f& get_viewport() const
    {
        return m_viewport;
    }

    void set_viewport(const arc::lx::AABB2f& viewport)
    {
        assert(viewport.min()(0) >= 0.0F);
        assert(viewport.min()(0) <= 1.0F);
        assert(viewport.min()(1) >= 0.0F);
        assert(viewport.min()(1) <= 1.0F);
        assert(viewport.max()(0) >= 0.0F);
        assert(viewport.max()(0) <= 1.0F);
        assert(viewport.max()(1) >= 0.0F);
        assert(viewport.max()(1) <= 1.0F);
        // has the viewport changed?
        if(m_viewport.min() != viewport.min() ||
           m_viewport.max() != viewport.max())
        {
            m_rebuild_surface = true;
        }
        m_viewport = viewport;
    }

    RenderMode get_render_modes() const
    {
        return m_render_modes;
    }

    void set_render_modes(RenderMode render_modes)
    {
        m_render_modes = render_modes;
    }

    const arc::lx::Vector3f& get_background_colour() const
    {
        return m_background_colour;
    }

    void set_background_colour(const arc::lx::Vector3f& colour)
    {
        m_background_colour = colour;
    }

    void render(death::Scene* scene)
    {
        // build the viewport surface (if required)
        build_viewport_surface();

        // calculate the current resolution
        arc::lx::Vector2u scene_resolution = scene->get_resolution();
        arc::lx::Vector2u origin(
            scene_resolution(0) * m_viewport.min()(0),
            scene_resolution(1) * m_viewport.min()(1)
        );
        arc::lx::Vector2u resolution(
            (scene_resolution(0) * m_viewport.max()(0)) - origin(0),
            (scene_resolution(1) * m_viewport.max()(1)) - origin(1)
        );

        // bail out with 0 resolution
        if(resolution(0) == 0 || resolution(1) == 0)
        {
            return;
        }

        // resolution changed?
        if(resolution != m_resolution)
        {
            m_resolution = resolution;

            // regenerate the frame buffer and the texture the components
            m_framebuffer.init(m_resolution);

            m_colour_component.init(
                GL_RGBA,
                m_resolution,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                nullptr
            );
            m_framebuffer.attach_colour_texture(m_colour_component);

            m_depth_component.init(
                GL_DEPTH_COMPONENT,
                m_resolution,
                GL_DEPTH_COMPONENT,
                GL_FLOAT,
                nullptr
            );
            m_framebuffer.attach_depth_texture(m_depth_component);
        }

        // bind the framebuffer to render to
        m_framebuffer.bind();

        // set up the graphics state for this view
        glClearColor(
            m_background_colour(0),
            m_background_colour(1),
            m_background_colour(2),
            1.0F
        );
        glViewport(
            0,
            0,
            m_resolution(0),
            m_resolution(1)
        );

        // clear the frame buffer to start rendering
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // handle the various render modes
        if(m_render_modes & kRenderModePathTracer)
        {
            death::PathTracer::instance().render(scene);
        }
        if(m_render_modes & kRenderModeGeometric)
        {
            death::GeometryRenderer::instance().render(scene);
        }
        if(m_render_modes & kRenderModeCell)
        {
            death::CellRenderer::instance().render(scene);
        }
        if(m_render_modes & kRenderModeBound)
        {
            death::BoundRenderer::instance().render(scene);
        }
        if(m_render_modes & kRenderModeOctree)
        {
            death::OctreeRenderer::instance().render(scene);
        }

        // TODO: control with something
        // death::OrientationRenderer::instance().render(scene);

        m_framebuffer.unbind();

        // reset the graphics state
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glViewport(0, 0, scene_resolution(0), scene_resolution(1));

        // render the screen space quad
        glBindVertexArray(m_vao);
        g_shader_program->bind();

        // pass in the colour texture
        glActiveTexture(GL_TEXTURE0);
        m_colour_component.bind();
        g_shader_program->set_uniform_1u("u_texture", 0);

        glDrawArrays(GL_TRIANGLES, 0, m_number_of_points);
        g_shader_program->unbind();
        glBindVertexArray(0);
    }

private:

    //------------P R I V A T E    M E M B E R    F U N C T I O N S-------------

    void build_viewport_surface()
    {
        // no work to do
        if(!m_rebuild_surface)
        {
            return;
        }
        m_rebuild_surface = false;

        // setup the shader once
        if(!g_shader_program)
        {
            // TODO: shaders should be built into the resource code
            arc::io::sys::Path vertex_path;
            vertex_path
                << "res" << "deathray" << "gl" << "shaders" << "vertex"
                << "view.glsl";
            arc::io::sys::Path fragment_path;
            fragment_path
                << "res" << "deathray" << "gl" << "shaders" << "fragment"
                << "view.glsl";

            g_shader_program.reset(new death::gl::ShaderProgram());
            g_shader_program->attach_shader_from_file(
                GL_VERTEX_SHADER,
                vertex_path
            );
            g_shader_program->attach_shader_from_file(
                GL_FRAGMENT_SHADER,
                fragment_path
            );
            g_shader_program->link();
        }

        float min_x = (m_viewport.min()(0) * 2.0F) - 1.0F;
        float max_x = (m_viewport.max()(0) * 2.0F) - 1.0F;
        float min_y = (m_viewport.min()(1) * 2.0F) - 1.0F;
        float max_y = (m_viewport.max()(1) * 2.0F) - 1.0F;

        // set up positions and uvs
        const std::vector<float> positions = {
            min_x, min_y, 0.0F,
            max_x, min_y, 0.0F,
            max_x, max_y, 0.0F,
            max_x, max_y, 0.0F,
            min_x, max_y, 0.0F,
            min_x, min_y, 0.0F
        };
        const std::vector<float> uvs = {
            0.0F, 0.0F,
            1.0F, 0.0F,
            1.0F, 1.0F,
            1.0F, 1.0F,
            0.0F, 1.0F,
            0.0F, 0.0F
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
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

View::View(
        const arc::lx::AABB2f& viewport,
        RenderMode render_modes,
        const arc::lx::Vector3f background_colour)
    : m_impl(new ViewImpl(viewport, render_modes, background_colour))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

View::~View()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

const arc::lx::AABB2f& View::get_viewport() const
{
    return m_impl->get_viewport();
}

void View::set_viewport(const arc::lx::AABB2f& viewport)
{
    m_impl->set_viewport(viewport);
}

View::RenderMode View::get_render_modes() const
{
    return m_impl->get_render_modes();
}

void View::set_render_modes(RenderMode render_modes)
{
    m_impl->set_render_modes(render_modes);
}

const arc::lx::Vector3f& View::get_background_colour() const
{
    return m_impl->get_background_colour();
}

void View::set_background_colour(const arc::lx::Vector3f& colour)
{
    m_impl->set_background_colour(colour);
}

void View::render(death::Scene* scene)
{
    m_impl->render(scene);
}

} // namespace death
