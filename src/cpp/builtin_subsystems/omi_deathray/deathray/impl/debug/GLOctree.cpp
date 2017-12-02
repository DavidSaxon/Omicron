/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/debug/GLOctree.hpp"

#include <vector>

#include <arcanecore/lx/Vector.hpp>

#include <GL/glew.h>

#include "deathray/impl/Octant.hpp"


namespace death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class GLOctree::GLOctreeImpl
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

    GLOctreeImpl(death::Octant* root)
        : m_vao             (0)
        , m_number_of_points(0)
    {
        // build the data
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        // calculate half size
        float half_root_size = root->get_size() / 2.0F;

        // create the top level bounding box data
        std::vector<float> positions = {
            -half_root_size, -half_root_size, -half_root_size,
             half_root_size, -half_root_size, -half_root_size,

             half_root_size, -half_root_size, -half_root_size,
             half_root_size,  half_root_size, -half_root_size,

             half_root_size,  half_root_size, -half_root_size,
            -half_root_size,  half_root_size, -half_root_size,

            -half_root_size,  half_root_size, -half_root_size,
            -half_root_size, -half_root_size, -half_root_size,

            -half_root_size, -half_root_size,  half_root_size,
             half_root_size, -half_root_size,  half_root_size,

             half_root_size, -half_root_size,  half_root_size,
             half_root_size,  half_root_size,  half_root_size,

             half_root_size,  half_root_size,  half_root_size,
            -half_root_size,  half_root_size,  half_root_size,

            -half_root_size,  half_root_size,  half_root_size,
            -half_root_size, -half_root_size,  half_root_size,

            -half_root_size, -half_root_size, -half_root_size,
            -half_root_size, -half_root_size,  half_root_size,

             half_root_size, -half_root_size, -half_root_size,
             half_root_size, -half_root_size,  half_root_size,

            -half_root_size,  half_root_size, -half_root_size,
            -half_root_size,  half_root_size,  half_root_size,

             half_root_size,  half_root_size, -half_root_size,
             half_root_size,  half_root_size,  half_root_size
        };
        // extend_with_octant(
        //     root,
        //     arc::lx::Vector3f(0.0F, 0.0F, 0.0F),
        //     positions
        // );
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

    //---------------------------D E S T R U C T O R----------------------------

    ~GLOctreeImpl()
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
        glDrawArrays(GL_LINES, 0, m_number_of_points);
        glBindVertexArray(0);
    }

private:

    //------------P R I V A T E    M E M B E R    F U N C T I O N S-------------

    void extend_with_octant(
            death::Octant* octant,
            const arc::lx::Vector3f& offset,
            std::vector<float>& positions)
    {
        // has depth?
        if(octant->is_leaf())
        {
            return;
        }

        float half_size = octant->get_size() / 2.0F;

        // x
        positions.push_back(offset(0) + -half_size);
        positions.push_back(offset(1) + 0);
        positions.push_back(offset(2) + 0);
        positions.push_back(offset(0) +  half_size);
        positions.push_back(offset(1) + 0);
        positions.push_back(offset(2) + 0);

        positions.push_back(offset(0) + -half_size);
        positions.push_back(offset(1) + -half_size);
        positions.push_back(offset(2) + 0);
        positions.push_back(offset(0) +  half_size);
        positions.push_back(offset(1) + -half_size);
        positions.push_back(offset(2) + 0);

        positions.push_back(offset(0) + -half_size);
        positions.push_back(offset(1) +  half_size);
        positions.push_back(offset(2) + 0);
        positions.push_back(offset(0) +  half_size);
        positions.push_back(offset(1) +  half_size);
        positions.push_back(offset(2) + 0);

        positions.push_back(offset(0) + -half_size);
        positions.push_back(offset(1) + 0);
        positions.push_back(offset(2) + -half_size);
        positions.push_back(offset(0) +  half_size);
        positions.push_back(offset(1) + 0);
        positions.push_back(offset(2) + -half_size);

        positions.push_back(offset(0) + -half_size);
        positions.push_back(offset(1) + 0);
        positions.push_back(offset(2) +  half_size);
        positions.push_back(offset(0) +  half_size);
        positions.push_back(offset(1) + 0);
        positions.push_back(offset(2) +  half_size);

        // y
        positions.push_back(offset(0) + 0);
        positions.push_back(offset(1) + -half_size);
        positions.push_back(offset(2) + 0);
        positions.push_back(offset(0) + 0);
        positions.push_back(offset(1) +  half_size);
        positions.push_back(offset(2) + 0);

        positions.push_back(offset(0) + -half_size);
        positions.push_back(offset(1) + -half_size);
        positions.push_back(offset(2) + 0);
        positions.push_back(offset(0) + -half_size);
        positions.push_back(offset(1) +  half_size);
        positions.push_back(offset(2) + 0);

        positions.push_back(offset(0) +  half_size);
        positions.push_back(offset(1) + -half_size);
        positions.push_back(offset(2) + 0);
        positions.push_back(offset(0) +  half_size);
        positions.push_back(offset(1) +  half_size);
        positions.push_back(offset(2) + 0);

        positions.push_back(offset(0) + 0);
        positions.push_back(offset(1) + -half_size);
        positions.push_back(offset(2) + -half_size);
        positions.push_back(offset(0) + 0);
        positions.push_back(offset(1) +  half_size);
        positions.push_back(offset(2) + -half_size);

        positions.push_back(offset(0) + 0);
        positions.push_back(offset(1) + -half_size);
        positions.push_back(offset(2) +  half_size);
        positions.push_back(offset(0) + 0);
        positions.push_back(offset(1) +  half_size);
        positions.push_back(offset(2) +  half_size);

        // z
        positions.push_back(offset(0) + 0);
        positions.push_back(offset(1) + 0);
        positions.push_back(offset(2) + -half_size);
        positions.push_back(offset(0) + 0);
        positions.push_back(offset(1) + 0);
        positions.push_back(offset(2) +  half_size);

        positions.push_back(offset(0) + -half_size);
        positions.push_back(offset(1) + 0);
        positions.push_back(offset(2) + -half_size);
        positions.push_back(offset(0) + -half_size);
        positions.push_back(offset(1) + 0);
        positions.push_back(offset(2) +  half_size);

        positions.push_back(offset(0) +  half_size);
        positions.push_back(offset(1) + 0);
        positions.push_back(offset(2) + -half_size);
        positions.push_back(offset(0) +  half_size);
        positions.push_back(offset(1) + 0);
        positions.push_back(offset(2) +  half_size);

        positions.push_back(offset(0) + 0);
        positions.push_back(offset(1) + -half_size);
        positions.push_back(offset(2) + -half_size);
        positions.push_back(offset(0) + 0);
        positions.push_back(offset(1) + -half_size);
        positions.push_back(offset(2) +  half_size);

        positions.push_back(offset(0) + 0);
        positions.push_back(offset(1) +  half_size);
        positions.push_back(offset(2) + -half_size);
        positions.push_back(offset(0) + 0);
        positions.push_back(offset(1) +  half_size);
        positions.push_back(offset(2) +  half_size);

        float quater_size = half_size / 2.0F;

        // recurse
        extend_with_octant(
            octant->get_child(false, false, false),
            arc::lx::Vector3f(
                offset(0) - quater_size,
                offset(1) - quater_size,
                offset(2) - quater_size
            ),
            positions
        );
        extend_with_octant(
            octant->get_child(true, false, false),
            arc::lx::Vector3f(
                offset(0) + quater_size,
                offset(1) - quater_size,
                offset(2) - quater_size
            ),
            positions
        );
        extend_with_octant(
            octant->get_child(false, true, false),
            arc::lx::Vector3f(
                offset(0) - quater_size,
                offset(1) + quater_size,
                offset(2) - quater_size
            ),
            positions
        );
        extend_with_octant(
            octant->get_child(true, true, false),
            arc::lx::Vector3f(
                offset(0) + quater_size,
                offset(1) + quater_size,
                offset(2) - quater_size
            ),
            positions
        );

        extend_with_octant(
            octant->get_child(false, false, true),
            arc::lx::Vector3f(
                offset(0) - quater_size,
                offset(1) - quater_size,
                offset(2) + quater_size
            ),
            positions
        );
        extend_with_octant(
            octant->get_child(true, false, true),
            arc::lx::Vector3f(
                offset(0) + quater_size,
                offset(1) - quater_size,
                offset(2) + quater_size
            ),
            positions
        );
        extend_with_octant(
            octant->get_child(false, true, true),
            arc::lx::Vector3f(
                offset(0) - quater_size,
                offset(1) + quater_size,
                offset(2) + quater_size
            ),
            positions
        );
        extend_with_octant(
            octant->get_child(true, true, true),
            arc::lx::Vector3f(
                offset(0) + quater_size,
                offset(1) + quater_size,
                offset(2) + quater_size
            ),
            positions
        );
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

GLOctree::GLOctree(death::Octant* root)
    : m_impl(new GLOctreeImpl(root))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

GLOctree::~GLOctree()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void GLOctree::draw()
{
    m_impl->draw();
}

} // namespace death
