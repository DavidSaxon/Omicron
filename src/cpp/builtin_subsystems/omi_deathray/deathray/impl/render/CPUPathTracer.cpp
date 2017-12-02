/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/render/CPUPathTracer.hpp"

#include <vector>

#include <arcanecore/lx/Alignment.hpp>
#include <arcanecore/lx/Vector.hpp>

#include "deathray/gl/ShaderProgram.hpp"
#include "deathray/gl/Texture2D.hpp"

#include "deathray/impl/Camera.hpp"
#include "deathray/impl/Scene.hpp"
#include "deathray/impl/pathtracer/cpu/CPULightPath.hpp"


namespace death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class CPUPathTracer::CPUPathTracerImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the buffer containing the rendered image plane data
    std::vector<DeathFloat> m_image_data;
    // the current resolution of the image data
    arc::lx::Vector2u m_current_resolution;

    // the current scanline being rendered
    std::size_t m_current_scanline;

    // the 2D texture containing the data that will be rendered to screen
    death::gl::Texture2D m_screen_texture;

    // the OpenGL shader program to render the image to the screen
    death::gl::ShaderProgram m_shader_program;
    // the OpenGL Vertex Array Object to use to render the image data
    GLuint m_vao;
    // the number of points in the image surface
    std::size_t m_number_of_points;

public:

    ARC_LX_ALIGNED_NEW;

    //--------------------------C O N S T R U C T O R---------------------------

    CPUPathTracerImpl()
        : m_current_resolution(0, 0)
        , m_current_scanline  (0)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~CPUPathTracerImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    void render(death::Scene* scene)
    {
        static_setup();

        bool resolution_change = false;

        // has the resolution changed?
        const arc::lx::Vector2u res = scene->get_resolution();
        if(res != m_current_resolution)
        {
            resolution_change = true;
            m_current_resolution = res;
            m_image_data.clear();
            m_image_data.resize(
                m_current_resolution(0) * m_current_resolution(1) * 3,
                0.0F
            );
            m_current_scanline = 0;
        }

        pathtrace(scene);

        // write to the screen texture
        write_to_texture(resolution_change);
        // render to screen space quad
        draw_image();
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
            << "cpu_pathtracer.glsl";
        arc::io::sys::Path fragment_path;
        fragment_path
            << "res" << "deathray" << "gl" << "shaders" << "fragment"
            << "cpu_pathtracer.glsl";

        m_shader_program.attach_shader_from_file(
            GL_VERTEX_SHADER,
            vertex_path
        );
        m_shader_program.attach_shader_from_file(
            GL_FRAGMENT_SHADER,
            fragment_path
        );
        m_shader_program.link();

        // set up positions and uvs for the render surface
        const std::vector<float> positions = {
            -1.0F, -1.0F, 0.0F,
             1.0F, -1.0F, 0.0F,
             1.0F,  1.0F, 0.0F,
             1.0F,  1.0F, 0.0F,
            -1.0F,  1.0F, 0.0F,
            -1.0F, -1.0F, 0.0F
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

    void pathtrace(death::Scene* scene)
    {
        // get the camera from the scene
        death::Camera* camera = scene->get_camera();

        static const std::size_t pixel_stride = 3;
        static const std::size_t row_stride =
            m_current_resolution(0) * pixel_stride;

        // render 8 scanline each cycle
        for(std::size_t y = 0; y < 8; ++y)
        {
            for(std::size_t x = 0; x < m_current_resolution(0); ++x)
            {
                // TODO: this should be based on sensor size
                arc::lx::Vector2f plane_position(
                    static_cast<float>(x),
                    static_cast<float>(m_current_scanline)
                );
                plane_position(0) /=
                    static_cast<float>(m_current_resolution(0));
                plane_position(1) /=
                    static_cast<float>(m_current_resolution(1));
                plane_position(0) *= 2.0F;
                plane_position(1) *= 2.0F;
                plane_position(0) -= 1.0F;
                plane_position(1) -= 1.0F;
                plane_position(0) *= 10.0F;
                plane_position(1) *= 10.0F;

                // create the path object
                death::cpu::LightPath path(
                    camera->get_focal_length(),
                    plane_position
                );

                // trace the path to get the image colour
                arc::lx::Vector3f image_colour = path.trace(scene, nullptr);

                // write to the image plane
                std::size_t address =
                    (m_current_scanline * row_stride) + (x * pixel_stride);
                m_image_data[address + 0] = image_colour(0);
                m_image_data[address + 1] = image_colour(1);
                m_image_data[address + 2] = image_colour(2);
            }

            // move to the next scanline
            ++m_current_scanline;
            if(m_current_scanline >= m_current_resolution(1))
            {
                m_current_scanline = 0;
            }
        }
    }

    // writes the current image data to the screen texture
    void write_to_texture(bool resolution_change)
    {
        if(resolution_change)
        {
            // write new texture data
            m_screen_texture.init(
                GL_RGBA,
                m_current_resolution,
                GL_RGB,
                GL_FLOAT,
                &m_image_data[0]
            );
        }
        else
        {
            arc::lx::Vector2u offset(0, 0);
            m_screen_texture.update(
                offset,
                m_current_resolution,
                GL_RGB,
                GL_FLOAT,
                &m_image_data[0]
            );
        }
    }

    // draws the CPU Path tracer image to the viewport
    void draw_image()
    {
        // reset the graphics state
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        // render the screen space quad
        glBindVertexArray(m_vao);
        m_shader_program.bind();

        // pass in the colour texture
        glActiveTexture(GL_TEXTURE0);
        m_screen_texture.bind();
        m_shader_program.set_uniform_1i("u_texture", 0);

        glDrawArrays(GL_TRIANGLES, 0, m_number_of_points);
        m_shader_program.unbind();
        glBindVertexArray(0);
    }
};

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

CPUPathTracer& CPUPathTracer::instance()
{
    static CPUPathTracer inst;
    return inst;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void CPUPathTracer::render(death::Scene* scene)
{
    m_impl->render(scene);
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

CPUPathTracer::CPUPathTracer()
    : m_impl(new CPUPathTracerImpl())
{
}

//------------------------------------------------------------------------------
//                               PRIVATE DESTRUCTOR
//------------------------------------------------------------------------------

CPUPathTracer::~CPUPathTracer()
{
    delete m_impl;
}

} // namespace death
