/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/data/PackedOctree.hpp"

#include <vector>

#include <arcanecore/lx/MatrixMath44f.hpp>

#include "deathray/impl/Globals.hpp"
#include "deathray/impl/Octant.hpp"
#include "deathray/impl/Octree.hpp"

namespace death
{

//------------------------------------------------------------------------------
//                                    GLOBALS
//------------------------------------------------------------------------------

namespace
{

// TODO:
static const std::size_t MAX_DATA_WIDTH = 256;
// the maximum data size (in 32-bit integers) that packed data can use
static const std::size_t MAX_DATA_SIZE = MAX_DATA_WIDTH * MAX_DATA_WIDTH;

} // namespace anonymous

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class PackedOctree::PackedOctreeImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // The Octree this data is built for
    death::Octree* m_octree;

    // the width of the data
    std::size_t m_width;

    // the raw packed data
    std::vector<DeathFloat> m_data;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    PackedOctreeImpl(death::Octree* octree)
        : m_octree(octree)
        , m_width (0)
    {
        // build the packed data for the first time
        build();
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~PackedOctreeImpl()
    {
        // TODO:
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    std::size_t get_data_stride() const
    {
        return 3;
    }

    std::size_t get_data_width() const
    {
        return m_width;
    }

    const DeathFloat* get_data() const
    {
        return &m_data[0];
    }

    DeathFloat get_value(std::size_t index) const
    {
        return m_data[index];
    }

    DeathFloat get_value(std::size_t x, std::size_t y) const
    {
        return get_value((y * m_width * 3) + (x * 3));
    }

    const arc::lx::Vector3f* get_value_3(std::size_t index) const
    {
        return reinterpret_cast<const arc::lx::Vector3f*>(&m_data[index]);
    }

    const arc::lx::Vector3f* get_value_3(std::size_t x, std::size_t y) const
    {
        return get_value_3((y * m_width * 3) + (x * 3));
    }

    DeathFloat get_root_size() const
    {
        return get_value(0);
    }

    arc::lx::Vector3f get_position() const
    {
        return (*get_value_3(1 * 3));
    }

    arc::lx::Vector3f get_rotation() const
    {
        return (*get_value_3(2 * 3));
    }

    arc::lx::Vector3f get_scale() const
    {
        return (*get_value_3(3 * 3));
    }

private:

    //------------P R I V A T E    M E M B E R    F U N C T I O N S-------------

    void build()
    {
        // setup buffer
        m_data.clear();
        m_data.reserve(MAX_DATA_SIZE * get_data_stride());

        // builder the header
        // ----
        // size
        m_data.push_back(m_octree->get_root_size());
        m_data.push_back(0.4F);
        m_data.push_back(0.0F); // unused
        // position
        // TODO: get more than just offset
        arc::lx::Vector3f offset = arc::lx::extract_translation_from_affine_44f(
            m_octree->get_offset()
        );
        m_data.push_back(offset(0));
        m_data.push_back(offset(1));
        m_data.push_back(offset(2));
        // rotation
        // TODO:
        m_data.push_back(0.0F);
        m_data.push_back(0.0F);
        m_data.push_back(0.0F);
        // scale
        // TODO:
        m_data.push_back(1.0F);
        m_data.push_back(1.0F);
        m_data.push_back(1.0F);

        // recursively inject octants
        inject_octant(m_octree->get_root());

        if(m_data.size() > MAX_DATA_SIZE)
        {
            DEATH_LOG_ERROR(
                "Octree: " << m_octree << " larger than maximum size: "
                << m_data.size()
            );
            return;
        }

        DEATH_LOG_DEBUG(
            "Octree: " << m_octree << " packed size: " << m_data.size()
        );
    }

    // recursively injects octants into the buffer and returns the address
    void inject_octant(death::Octant* octant)
    {
        // TODO: currently not utilising the sign bit

        // leaf level?
        if(octant->is_leaf())
        {
            // any data here?
            if(octant->has_data())
            {
                // TODO: encode shading here
                m_data.push_back(1.0F);
            }
            else
            {
                m_data.push_back(0.0F);
            }
        }
        else
        {
            std::size_t this_address = m_data.size();
            // inject place holder data for this octant's address
            for(std::size_t i = 0; i < 8; ++i)
            {
                m_data.push_back(0.0F);
            }

            // build each child
            // -x -y -z
            inject_octant_or_null(
                octant->get_child(false, false, false),
                this_address + 0
            );
            // x -y -z
            inject_octant_or_null(
                octant->get_child(true, false, false),
                this_address + 1
            );
            // -x  y -z
            inject_octant_or_null(
                octant->get_child(false, true, false),
                this_address + 2
            );
            // x  y -z
            inject_octant_or_null(
                octant->get_child(true, true, false),
                this_address + 3
            );
            // -x -y z
            inject_octant_or_null(
                octant->get_child(false, false, true),
                this_address + 4
            );
            // x -y  z
            inject_octant_or_null(
                octant->get_child(true, false, true),
                this_address + 5
            );
            // -x  y  z
            inject_octant_or_null(
                octant->get_child(false, true, true),
                this_address + 6
            );
            // x  y  z
            inject_octant_or_null(
                octant->get_child(true, true, true),
                this_address + 7
            );
        }
    }

    void inject_octant_or_null(death::Octant* octant, size_t address)
    {
        // leaf level?
        if(octant->is_leaf() && !octant->has_data())
        {
            m_data[address] = 0.0F;
        }
        else
        {
            m_data[address] = m_data.size();
            inject_octant(octant);
        }
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

PackedOctree::PackedOctree(death::Octree* octree)
    : m_impl(new PackedOctreeImpl(octree))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

PackedOctree::~PackedOctree()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

std::size_t PackedOctree::get_data_stride() const
{
    return m_impl->get_data_stride();
}

std::size_t PackedOctree::get_data_width() const
{
    return m_impl->get_data_width();
}

const DeathFloat* PackedOctree::get_data() const
{
    return m_impl->get_data();
}

DeathFloat PackedOctree::get_value(std::size_t index) const
{
    return m_impl->get_value(index);
}

DeathFloat PackedOctree::get_value(std::size_t x, std::size_t y) const
{
    return m_impl->get_value(x, y);
}

const arc::lx::Vector3f* PackedOctree::get_value_3(std::size_t index) const
{
    return m_impl->get_value_3(index);
}

const arc::lx::Vector3f* PackedOctree::get_value_3(
        std::size_t x,
        std::size_t y) const
{
    return m_impl->get_value_3(x, y);
}

DeathFloat PackedOctree::get_root_size() const
{
    return m_impl->get_root_size();
}

arc::lx::Vector3f PackedOctree::get_position() const
{
    return m_impl->get_position();
}

arc::lx::Vector3f PackedOctree::get_rotation() const
{
    return m_impl->get_rotation();
}

arc::lx::Vector3f PackedOctree::get_scale() const
{
    return m_impl->get_scale();
}


} // namespace death
