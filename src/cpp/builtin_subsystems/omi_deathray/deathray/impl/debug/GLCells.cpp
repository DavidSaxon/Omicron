/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/debug/GLCells.hpp"

#include <vector>

#include <arcanecore/lx/Vector.hpp>

#include <GL/glew.h>

#include "deathray/impl/Octant.hpp"


namespace death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class GLCells::GLCellsImpl
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

    GLCellsImpl(death::Octant* root)
        : m_vao             (0)
        , m_number_of_points(0)
    {
        // build the data
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        // build the geometry and colours
        std::vector<float> positions;
        std::vector<float> colours;
        extend_with_octant(
            root,
            arc::lx::Vector3f(0.0F, 0.0F, 0.0F),
            positions,
            colours
        );
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

    //---------------------------D E S T R U C T O R----------------------------

    ~GLCellsImpl()
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

private:

    //------------P R I V A T E    M E M B E R    F U N C T I O N S-------------

    void extend_with_octant(
            death::Octant* octant,
            const arc::lx::Vector3f& offset,
            std::vector<float>& positions,
            std::vector<float>& colours)
    {
        float half_size = octant->get_size() / 2.0F;
        // only generate for leaf level
        if(!octant->is_leaf())
        {
            float quater_size = half_size / 2.0F;
            // recurse
            extend_with_octant(
                octant->get_child(false, false, false),
                arc::lx::Vector3f(
                    offset(0) - quater_size,
                    offset(1) - quater_size,
                    offset(2) - quater_size
                ),
                positions,
                colours
            );
            extend_with_octant(
                octant->get_child(true, false, false),
                arc::lx::Vector3f(
                    offset(0) + quater_size,
                    offset(1) - quater_size,
                    offset(2) - quater_size
                ),
                positions,
                colours
            );
            extend_with_octant(
                octant->get_child(false, true, false),
                arc::lx::Vector3f(
                    offset(0) - quater_size,
                    offset(1) + quater_size,
                    offset(2) - quater_size
                ),
                positions,
                colours
            );
            extend_with_octant(
                octant->get_child(true, true, false),
                arc::lx::Vector3f(
                    offset(0) + quater_size,
                    offset(1) + quater_size,
                    offset(2) - quater_size
                ),
                positions,
                colours
            );

            extend_with_octant(
                octant->get_child(false, false, true),
                arc::lx::Vector3f(
                    offset(0) - quater_size,
                    offset(1) - quater_size,
                    offset(2) + quater_size
                ),
                positions,
                colours
            );
            extend_with_octant(
                octant->get_child(true, false, true),
                arc::lx::Vector3f(
                    offset(0) + quater_size,
                    offset(1) - quater_size,
                    offset(2) + quater_size
                ),
                positions,
                colours
            );
            extend_with_octant(
                octant->get_child(false, true, true),
                arc::lx::Vector3f(
                    offset(0) - quater_size,
                    offset(1) + quater_size,
                    offset(2) + quater_size
                ),
                positions,
                colours
            );
            extend_with_octant(
                octant->get_child(true, true, true),
                arc::lx::Vector3f(
                    offset(0) + quater_size,
                    offset(1) + quater_size,
                    offset(2) + quater_size
                ),
                positions,
                colours
            );
            return;
        }

        if(!octant->has_data())
        {
            return;
        }

        // build positions
        float min_x = offset(0) - half_size;
        float min_y = offset(1) - half_size;
        float min_z = offset(2) - half_size;
        float max_x = offset(0) + half_size;
        float max_y = offset(1) + half_size;
        float max_z = offset(2) + half_size;
        const std::vector<float> cell_positions
        {
            min_x, min_y, max_z,
            max_x, min_y, max_z,
            max_x, max_y, max_z,
            max_x, max_y, max_z,
            min_x, max_y, max_z,
            min_x, min_y, max_z,

            max_x, min_y, min_z,
            max_x, max_y, min_z,
            max_x, max_y, max_z,
            max_x, max_y, max_z,
            max_x, min_y, max_z,
            max_x, min_y, min_z,

            max_x, max_y, min_z,
            max_x, min_y, min_z,
            min_x, min_y, min_z,
            min_x, min_y, min_z,
            min_x, max_y, min_z,
            max_x, max_y, min_z,

            min_x, max_y, max_z,
            min_x, max_y, min_z,
            min_x, min_y, min_z,
            min_x, min_y, min_z,
            min_x, min_y, max_z,
            min_x, max_y, max_z,

            max_x, max_y, max_z,
            max_x, max_y, min_z,
            min_x, max_y, min_z,
            min_x, max_y, min_z,
            min_x, max_y, max_z,
            max_x, max_y, max_z,

            min_x, min_y, min_z,
            max_x, min_y, min_z,
            max_x, min_y, max_z,
            max_x, min_y, max_z,
            min_x, min_y, max_z,
            min_x, min_y, min_z
        };
        positions.insert(
            positions.end(),
            cell_positions.begin(),
            cell_positions.end()
        );

        // build colours
        Octant::Id id = octant->get_id();
        float id_r = static_cast<float>(id.bytes[0]) / 255.0F;
        float id_g = static_cast<float>(id.bytes[1]) / 255.0F;
        float id_b = static_cast<float>(id.bytes[2]) / 255.0F;
        const std::vector<float> cell_colours
        {
            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,

            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,

            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,

            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,

            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,

            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b,
            id_r, id_g, id_b
        };
        colours.insert(
            colours.end(),
            cell_colours.begin(),
            cell_colours.end()
        );
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

GLCells::GLCells(death::Octant* root)
    : m_impl(new GLCellsImpl(root))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

GLCells::~GLCells()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void GLCells::draw()
{
    m_impl->draw();
}

} // namespace death
