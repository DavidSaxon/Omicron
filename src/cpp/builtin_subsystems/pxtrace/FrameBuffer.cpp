#include "pxtrace/FrameBuffer.hpp"

#include <arcanecore/io/sys/FileReader.hpp>

#include "pxtrace/PXGlobals.hpp"


//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

FrameBuffer::FrameBuffer(const arc::gm::Vector2u& dimensions)
    : m_dimensions    (dimensions)
    , m_vao           (0)
    , m_vertex_buffer (0)
    , m_uv_buffer     (0)
    , m_shader_program(0)
    , m_texture       (0)
{
    init();
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

FrameBuffer::~FrameBuffer()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void FrameBuffer::render()
{
    // TODO: REMOVE ME
    // update the texture data
    for(std::size_t i = 0; i < m_dimensions.x() * m_dimensions.y(); ++i)
    {
        m_texture_data[(i * 3) + 0] = rand() % 256;
        m_texture_data[(i * 3) + 1] = rand() % 256;
        m_texture_data[(i * 3) + 2] = rand() % 256;
    }

    // update the OPenGL texture
    glTexSubImage2D(
        GL_TEXTURE_2D,
        0,
        0,
        0,
        m_dimensions.x(),
        m_dimensions.y(),
        GL_RGB,
        GL_UNSIGNED_BYTE,
        (GLvoid*) &m_texture_data[0]
    );

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glBindVertexArray(m_vao);
    glUseProgram(m_shader_program);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

//------------------------------------------------------------------------------
//                            PRIVATE MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void FrameBuffer::init()
{
    glViewport(0, 0, m_dimensions.x(), m_dimensions.y());

    glClearColor(0.0F, 0.0F, 0.0F, 1.0F);

    // setup for the vertex array object for the textured quad
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // TODO:
    // setup the vertices for the quad
    const GLfloat quad_vertices[] =
    {
        -1.0F, -1.0F, 0.0F,
         1.0F, -1.0F, 0.0F,
        -1.0F,  1.0F, 0.0F,
        -1.0F,  1.0F, 0.0F,
         1.0F,  1.0F, 0.0F,
         1.0F, -1.0F, 0.0F

    };
    glGenBuffers(1, &m_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(quad_vertices),
        quad_vertices,
        GL_STATIC_DRAW
    );

    // TODO:
    // setup uvs for the quad
    const GLfloat quad_uvs[] =
    {
        0.0F, 0.0F,
        1.0F, 0.0F,
        0.0F, 1.0F,
        0.0F, 1.0F,
        1.0F, 1.0F,
        1.0F, 0.0F
    };
    glGenBuffers(1, &m_uv_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_uv_buffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(quad_uvs),
        quad_uvs,
        GL_STATIC_DRAW
    );

    // set the vertex attributes
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    glVertexAttribPointer(
        0,                  // attribute 0
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*) 0           // array buffer offset
    );
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_uv_buffer);
    glVertexAttribPointer(
        1,                  // attribute 1
        2,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        (void*) 0           // array buffer offset
    );
    glBindVertexArray(0);

    // compile the shaders
    compile_shaders();

    // setup the texture data
    m_texture_data.resize(m_dimensions.x() * m_dimensions.y() * 3, 0);
    // TODO: REMOVE ME
    for(std::size_t i = 0; i < m_dimensions.x() * m_dimensions.y(); ++i)
    {
        m_texture_data[(i * 3) + 0] = rand() % 256;
        m_texture_data[(i * 3) + 1] = rand() % 256;
        m_texture_data[(i * 3) + 2] = rand() % 256;
    }

    // setup the texture
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        3,
        m_dimensions.x(),
        m_dimensions.y(),
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        (GLvoid*) &m_texture_data[0]
    );

    // Set up the texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

void FrameBuffer::compile_shaders()
{
    GLuint vertex_shader_id   = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    // read the vertex shader
    arc::io::sys::Path vertex_path;
    vertex_path << "res" << "pxtrace" << "shaders" << "framebuffer.vertex.glsl";
    arc::io::sys::FileReader vertex_reader(vertex_path);
    arc::str::UTF8String vertex_contents;
    vertex_reader.read(vertex_contents);

    // error status
    GLint status = GL_FALSE;
    int error_length = 0;
    // compile the vertex shader
    const char* vertex_source = vertex_contents.get_raw();
    glShaderSource(vertex_shader_id, 1, &vertex_source, nullptr);
    glCompileShader(vertex_shader_id);
    // check for errors
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &status);
    glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &error_length);
    if(status != GL_TRUE && error_length > 0)
    {
        std::vector<char> error_message(error_length + 1);
        glGetShaderInfoLog(
            vertex_shader_id,
            error_length,
            nullptr,
            &error_message[0]
        );
        global::logger->error
            << "Error while compiling framebuffer vertex shader: "
            << (&error_message[0]) << std::endl;
    }

    // read the fragment shader
    arc::io::sys::Path fragment_path;
    fragment_path
        << "res" << "pxtrace" << "shaders" << "framebuffer.fragment.glsl";
    arc::io::sys::FileReader fragment_reader(fragment_path);
    arc::str::UTF8String fragment_contents;
    fragment_reader.read(fragment_contents);

    // reset error status
    status = GL_FALSE;
    error_length = 0;
    // compile the fragment shader
    const char* fragment_source = fragment_contents.get_raw();
    glShaderSource(fragment_shader_id, 1, &fragment_source, nullptr);
    glCompileShader(fragment_shader_id);
    // check for errors
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &status);
    glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &error_length);
    if(status != GL_TRUE && error_length > 0)
    {
        std::vector<char> error_message(error_length + 1);
        glGetShaderInfoLog(
            fragment_shader_id,
            error_length,
            nullptr,
            &error_message[0]
        );
        global::logger->error
            << "Error while compiling framebuffer fragment shader: "
            << (&error_message[0]) << std::endl;
    }

    // reset error status
    status = GL_FALSE;
    error_length = 0;
    // link the program
    m_shader_program = glCreateProgram();
    glAttachShader(m_shader_program, vertex_shader_id);
    glAttachShader(m_shader_program, fragment_shader_id);
    glLinkProgram(m_shader_program);
    // check for errors
    glGetProgramiv(m_shader_program, GL_LINK_STATUS, &status);
    glGetProgramiv(m_shader_program, GL_INFO_LOG_LENGTH, &error_length);
    if(status != GL_TRUE && error_length > 0)
    {
        std::vector<char> error_message(error_length + 1);
        glGetProgramInfoLog(
            m_shader_program,
            error_length,
            nullptr,
            &error_message[0]
        );
        global::logger->error
            << "Error while linking framebuffer shader program: "
            << (&error_message[0]) << std::endl;
    }

    // TODO: ?
    // clean up
    glDetachShader(m_shader_program, vertex_shader_id);
    glDetachShader(m_shader_program, fragment_shader_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
}
