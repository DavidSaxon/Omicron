/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/render/PathTracer.hpp"

#include <GL/glew.h>

#include <deathray/gl/ShaderProgram.hpp>
#include <deathray/gl/Texture2D.hpp>

#include <deathray/image/PNG.hpp>

#include "deathray/impl/Camera.hpp"
#include "deathray/impl/Octree.hpp"
#include "deathray/impl/Scene.hpp"
#include "deathray/impl/Spatial.hpp"


namespace death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class PathTracer::PathTracerImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the OpenGL shader program for DeathRay
    death::gl::ShaderProgram m_shader_program;
    // TODO: TESTING
    // the IBL texture
    death::gl::Texture2D m_ibl_texture;
    // the normal map texture
    death::gl::Texture2D m_nmap_texture;
    // the test texture
    death::gl::Texture2D m_test_texture;

    // the vertex array object of the surface geometry
    GLuint m_surface_vao;
    // the number of points in the surface geometry
    std::size_t m_surface_points;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    PathTracerImpl()
        : m_surface_vao   (0)
        , m_surface_points(0)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~PathTracerImpl()
    {
        // release bound geometry
        m_surface_points = 0;
        if(m_surface_vao != 0)
        {
            glDeleteVertexArrays(1, &m_surface_vao);
            m_surface_vao = 0;
        }
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    void render(death::Scene* scene)
    {
        // perform setup once
        static_setup();

        // set state
        glDisable(GL_DEPTH_TEST);

        // bind
        glBindVertexArray(m_surface_vao);
        m_shader_program.bind();

        // pass in camera data
        death::Camera* camera = scene->get_camera();
        m_shader_program.set_uniform_44f(
            "u_camera_postion",
            camera->get_transform()
        );

        // pass in ibl data
        glActiveTexture(GL_TEXTURE0);
        m_ibl_texture.bind();
        m_shader_program.set_uniform_1i("u_ibl_texture", 0);

        // pass in normal map data
        glActiveTexture(GL_TEXTURE1);
        m_nmap_texture.bind();
        m_shader_program.set_uniform_1i("u_nmap_texture", 1);

        // pass in test texture data
        glActiveTexture(GL_TEXTURE2);
        m_test_texture.bind();
        m_shader_program.set_uniform_1i("u_test_texture", 2);

        // draw
        glDrawArrays(GL_TRIANGLES, 0, m_surface_points);

        // unbind
        m_shader_program.unbind();
        glBindVertexArray(0);
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
            << "pathtracer.glsl";
        arc::io::sys::Path fragment_path;
        fragment_path
            << "res" << "deathray" << "gl" << "shaders" << "fragment"
            << "pathtracer.glsl";

        m_shader_program.attach_shader_from_file(
            GL_VERTEX_SHADER,
            vertex_path
        );
        m_shader_program.attach_shader_from_file(
            GL_FRAGMENT_SHADER,
            fragment_path
        );
        m_shader_program.link();

        // TESTING
        //----------------------------------------------------------------------
        // load IBL data
        arc::io::sys::Path ibl_path;
        ibl_path
            << "res" << "builtin" << "texture" << "ibl" << "oribi.png";
        std::size_t ibl_width  = 0;
        std::size_t ibl_height = 0;
        bool ibl_has_alpha = false;
        unsigned char* ibl_data = nullptr;
        death::image::png::load_file(
            ibl_path,
            ibl_width,
            ibl_height,
            ibl_has_alpha,
            ibl_data
        );

        GLint ibl_format = GL_RGB;
        if(ibl_has_alpha)
        {
            ibl_format = GL_RGBA;
        }

        m_ibl_texture.init(
            ibl_format,
            arc::lx::Vector2u(ibl_width, ibl_height),
            ibl_format,
            GL_UNSIGNED_BYTE,
            ibl_data
        );

        // clean up
        delete[] ibl_data;
        //----------------------------------------------------------------------

        // load normal map data
        arc::io::sys::Path nmap_path;
        nmap_path
            << "res" << "builtin" << "texture" << "normal_map" << "sharp.png";
        std::size_t nmap_width  = 0;
        std::size_t nmap_height = 0;
        bool nmap_has_alpha = false;
        unsigned char* nmap_data = nullptr;
        death::image::png::load_file(
            nmap_path,
            nmap_width,
            nmap_height,
            nmap_has_alpha,
            nmap_data
        );

        GLint nmap_format = GL_RGB;
        if(nmap_has_alpha)
        {
            nmap_format = GL_RGBA;
        }

        m_nmap_texture.init(
            nmap_format,
            arc::lx::Vector2u(nmap_width, nmap_height),
            nmap_format,
            GL_UNSIGNED_BYTE,
            nmap_data
        );

        // clean up
        delete[] nmap_data;
        //----------------------------------------------------------------------

        // load test data
        arc::io::sys::Path test_path;
        test_path
            << "res" << "builtin" << "texture" << "test" << "space_1.png";
        std::size_t test_width  = 0;
        std::size_t test_height = 0;
        bool test_has_alpha = false;
        unsigned char* test_data = nullptr;
        death::image::png::load_file(
            test_path,
            test_width,
            test_height,
            test_has_alpha,
            test_data
        );

        GLint test_format = GL_RGB;
        if(test_has_alpha)
        {
            test_format = GL_RGBA;
        }

        m_test_texture.init(
            test_format,
            arc::lx::Vector2u(test_width, test_height),
            test_format,
            GL_UNSIGNED_BYTE,
            test_data
        );

        // clean up
        delete[] test_data;
        //----------------------------------------------------------------------

        // setup positions
        const std::vector<float> positions =
        {
            -1.0F, -1.0F, 0.0F,
             1.0F, -1.0F, 0.0F,
             1.0F,  1.0F, 0.0F,
             1.0F,  1.0F, 0.0F,
            -1.0F,  1.0F, 0.0F,
            -1.0F, -1.0F, 0.0F
        };
        const std::vector<float> image_plane =
        {
            -1.0F, -1.0F,
             1.0F, -1.0F,
             1.0F,  1.0F,
             1.0F,  1.0F,
            -1.0F,  1.0F,
            -1.0F, -1.0F
        };
        m_surface_points = positions.size() / 3;

        glGenVertexArrays(1, &m_surface_vao);
        glBindVertexArray(m_surface_vao);

        GLuint position_buffer = 0;
        glGenBuffers(1, &position_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
        glBufferData(
            GL_ARRAY_BUFFER,
            positions.size() * sizeof(float),
            &positions[0],
            GL_STATIC_DRAW
        );

        GLuint image_plane_buffer = 0;
        glGenBuffers(1, &image_plane_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, image_plane_buffer);
        glBufferData(
            GL_ARRAY_BUFFER,
            image_plane.size() * sizeof(float),
            &image_plane[0],
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
        glBindBuffer(GL_ARRAY_BUFFER, image_plane_buffer);
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
        glDeleteBuffers(1, &image_plane_buffer);
        glDeleteBuffers(1, &position_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
};

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

PathTracer& PathTracer::instance()
{
    static PathTracer inst;
    return inst;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void PathTracer::render(death::Scene* scene)
{
    m_impl->render(scene);
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

PathTracer::PathTracer()
    : m_impl(new PathTracerImpl())
{
}

//------------------------------------------------------------------------------
//                               PRIVATE DESTRUCTOR
//------------------------------------------------------------------------------

PathTracer::~PathTracer()
{
    delete m_impl;
}

} // namespace death
