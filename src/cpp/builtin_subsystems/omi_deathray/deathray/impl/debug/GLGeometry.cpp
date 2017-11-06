/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/debug/GLGeometry.hpp"

#include <GL/glew.h>


namespace death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class GLGeometry::GLGeometryImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the vertex array object for the geometry
    GLuint m_vao;
    // the number of points in the geometry
    DeathSize m_number_of_points;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    GLGeometryImpl(const DeathFloat* data, DeathSize size)
        : m_vao             (0)
        , m_number_of_points(size / 3)
    {
        // build that data
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        GLuint position_buffer = 0;
        glGenBuffers(1, &position_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
        glBufferData(
            GL_ARRAY_BUFFER,
            size * sizeof(float),
            data,
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

    //---------------------------D E S T R U C T O R----------------------------

    ~GLGeometryImpl()
    {
        m_number_of_points = 0;
        if(m_vao != 0)
        {
            glDeleteVertexArrays(1, &m_vao);
            m_vao = 0;
        }
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    void draw()
    {
        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, m_number_of_points);
        glBindVertexArray(0);
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

GLGeometry::GLGeometry(const DeathFloat* data, DeathSize size)
    : m_impl(new GLGeometryImpl(data, size))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

GLGeometry::~GLGeometry()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void GLGeometry::draw()
{
    m_impl->draw();
}

} // namespace death
