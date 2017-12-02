/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/acceleration/GPUOctree.hpp"

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
// the maximum data size (in 32-bit integers) that an Octree can use on the GPU
static const std::size_t MAX_DATA_SIZE = MAX_DATA_WIDTH * MAX_DATA_WIDTH;

} // namespace anonymous

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class GPUOctree::GPUOctreeImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // The Octree this GPU data is built for
    death::Octree* m_octree;

    // the GPU data
    death::gl::Texture2D m_gpu_buffer;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    GPUOctreeImpl(death::Octree* octree)
        : m_octree(octree)
    {
        // build the GPU` data for the first time
        build();
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~GPUOctreeImpl()
    {
        // TODO:
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    std::size_t get_buffer_width() const
    {
        return MAX_DATA_WIDTH;
    }

    death::gl::Texture2D& get_gpu_buffer()
    {
        return m_gpu_buffer;
    }

private:

    //------------P R I V A T E    M E M B E R    F U N C T I O N S-------------

    // builds the GPU data
    void build()
    {
        DEATH_LOG_DEBUG(
            "Building GPU data for Octree: " << m_octree
        );

        // TODO: this should actually build into a texture - should just the
        //       the raw data

        // TODO: the size may be able to calculated up front - or this data
        //       should be memory pooled
        // allocate the full size data
        std::vector<DeathFloat> buffer;
        buffer.reserve(MAX_DATA_SIZE);

        // build the header
        // TODO: position
        buffer.push_back(m_octree->get_root_size());
        // TODO: get this from somewhere
        buffer.push_back(0.04F);

        // inject octants into to the data.
        inject_octant(m_octree->get_root(), buffer);

        // check the size of allocated data
        if(buffer.size() > MAX_DATA_SIZE)
        {
            DEATH_LOG_ERROR(
                "Octree: " << m_octree << " larger than maximum size for " <<
                "transfer to the GPU: " << buffer.size()
            );
        }

        DEATH_LOG_DEBUG(
            "Octree: " << m_octree << " GPU size: " << buffer.size()
        );

        // transfer to the GPU texture buffer
        m_gpu_buffer.init(
            GL_R32F,
            arc::lx::Vector2u(MAX_DATA_WIDTH, MAX_DATA_WIDTH),
            GL_RED,
            GL_FLOAT,
            &buffer[0]
        );
    }

    // recursively injects octants into the buffer and returns the address
    void inject_octant(death::Octant* octant, std::vector<DeathFloat>& buffer)
    {
        // TODO: currently not utilising the sign bit

        // leaf level?
        if(octant->is_leaf())
        {
            // any data here?
            if(octant->has_data())
            {
                // TODO: encode shading here
                buffer.push_back(1.0F);
            }
            else
            {
                // TODO: REMOVE ME
                DEATH_LOG_DEBUG(
                    "ADDED NULL AT: " << buffer.size()
                );

                buffer.push_back(0.0F);
            }
        }
        else
        {
            std::size_t this_address = buffer.size();
            // inject place holder data for this octant's address
            for(std::size_t i = 0; i < 8; ++i)
            {
                buffer.push_back(0.0F);
            }

            // build each child
            // -x -y -z
            inject_octant_or_null(
                octant->get_child(false, false, false),
                this_address + 0,
                buffer
            );
            // x -y -z
            inject_octant_or_null(
                octant->get_child(true, false, false),
                this_address + 1,
                buffer
            );
            // -x  y -z
            inject_octant_or_null(
                octant->get_child(false, true, false),
                this_address + 2,
                buffer
            );
            // x  y -z
            inject_octant_or_null(
                octant->get_child(true, true, false),
                this_address + 3,
                buffer
            );
            // -x -y z
            inject_octant_or_null(
                octant->get_child(false, false, true),
                this_address + 4,
                buffer
            );
            // x -y  z
            inject_octant_or_null(
                octant->get_child(true, false, true),
                this_address + 5,
                buffer
            );
            // -x  y  z
            inject_octant_or_null(
                octant->get_child(false, true, true),
                this_address + 6,
                buffer
            );
            // x  y  z
            inject_octant_or_null(
                octant->get_child(true, true, true),
                this_address + 7,
                buffer
            );
        }
    }

    void inject_octant_or_null(
        death::Octant* octant,
        size_t address,
        std::vector<DeathFloat>& buffer)
    {
        // leaf level?
        if(octant->is_leaf() && !octant->has_data())
        {
            buffer[address] = 0.0F;
        }
        else
        {
            buffer[address] = buffer.size();
            inject_octant(octant, buffer);
        }
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

GPUOctree::GPUOctree(death::Octree* octree)
    : m_impl(new GPUOctreeImpl(octree))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

GPUOctree::~GPUOctree()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

std::size_t GPUOctree::get_buffer_width() const
{
    return m_impl->get_buffer_width();
}

death::gl::Texture2D& GPUOctree::get_gpu_buffer()
{
    return m_impl->get_gpu_buffer();
}

} // namespace death
