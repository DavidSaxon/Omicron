/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/Octree.hpp"

#include <memory>

#include <arcanecore/lx/Alignment.hpp>
#include <arcanecore/lx/MatrixMath44f.hpp>
#include <arcanecore/lx/Vector.hpp>

#include "deathray/impl/Geometric.hpp"
#include "deathray/impl/Octant.hpp"
#include "deathray/impl/Spatial.hpp"
#include "deathray/impl/VBO.hpp"
#include "deathray/impl/debug/GLCells.hpp"
#include "deathray/impl/debug/GLOctree.hpp"


namespace death
{

//------------------------------------------------------------------------------
//                                    GLOBALS
//------------------------------------------------------------------------------

namespace
{

// TODO: define this somewhere else
static const float MIN_SIZE = 0.04F;

} // namespace anonymous

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class Octree::OctreeImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // The spatial this Octree is built for
    death::Spatial* m_spatial;

    // the root octant of the octree
    std::unique_ptr<Octant> m_root;

    // defines whether this Octree has no renderable data
    bool m_empty;
    // the bounding box which exactly contains the data of this spatial
    arc::lx::AABB3f m_true_bounds;
    // the size of the octree
    float m_root_size;
    // the depth of the octree (from MIN_SIZE to root size)
    DeathSize m_depth;

    // TODO: clean up
    arc::lx::Vector3f m_offset_trans;
    // the matrix used to offset the octree to the spatial entities original
    // position
    arc::lx::Matrix44f m_offset;

    // the debug gl octree
    death::GLOctree* m_debug_octree;
    // the debug gl cell
    death::GLCells* m_debug_cells;

public:

    ARC_LX_ALIGNED_NEW;

    //--------------------------C O N S T R U C T O R---------------------------

    OctreeImpl(death::Spatial* spatial)
        : m_spatial     (spatial)
        , m_root        (nullptr)
        , m_empty       (true)
        , m_root_size   (0.0F)
        , m_depth       (0)
        , m_offset_trans(0.0F, 0.0F, 0.0F)
        , m_offset      (arc::lx::Matrix44f::Identity())
        , m_debug_octree(nullptr)
        , m_debug_cells (nullptr)
    {
        // build the octree data for the first time
        build();
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~OctreeImpl()
    {
        // TODO: need a graphics state queue for this
        if(m_debug_octree != nullptr)
        {
            delete m_debug_octree;
            m_debug_octree = nullptr;
        }
        if(m_debug_cells != nullptr)
        {
            delete m_debug_cells;
            m_debug_cells = nullptr;
        }
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    bool is_empty() const
    {
        return m_empty;
    }

    const arc::lx::AABB3f& get_true_bounds() const
    {
        return m_true_bounds;
    }

    float get_root_size() const
    {
        return m_root_size;
    }

    const arc::lx::Matrix44f& get_offset() const
    {
        return m_offset;
    }

    death::GLOctree* get_debug_octree()
    {
        // already exists?
        if(m_debug_octree != nullptr)
        {
            return m_debug_octree;
        }
        // empty?
        if(m_empty)
        {
            return nullptr;
        }

        // build
        m_debug_octree = new GLOctree(m_root.get());
        return m_debug_octree;
    }

    death::GLCells* get_debug_cells()
    {
        // already exists?
        if(m_debug_cells != nullptr)
        {
            return m_debug_cells;
        }
        // empty?
        if(m_empty)
        {
            return nullptr;
        }

        // build
        m_debug_cells = new GLCells(m_root.get());
        return m_debug_cells;
    }

private:

    //------------P R I V A T E    M E M B E R    F U N C T I O N S-------------

    // builds the data for this Octree
    void build()
    {
        // reset data
        m_root.reset(nullptr);
        m_empty = true;
        m_true_bounds.setEmpty();
        m_root_size = 0.0F;
        m_depth = 0;
        m_offset_trans = arc::lx::Vector3f(0.0F, 0.0F, 0.0F);
        m_offset = arc::lx::Matrix44f::Identity();
        // TODO:

        // we must run a first pass to find the bounding boxes of the spatial
        build_bounding_boxes();

        // bail out if there's no renderable data
        if(m_empty)
        {
            return;
        }

        // build the actual octree
        build_octants();
    }

    // builds the bounding box data for this octree
    void build_bounding_boxes()
    {
        for(death::Geometric* geometric : m_spatial->get_geometrics())
        {
            // get the primary VBO from the geometry
            death::VBO* vbo = geometric->get_vbo(0);
            // no primary vbo?
            if(vbo == nullptr)
            {
                continue;
            }
            // vbo tuple size not 3?
            if(vbo->get_tuple_size() != 3)
            {
                // TODO: should set an error here?
                continue;
            }
            // assume this is position data and traverse it find the spatial
            // bounds of this entity
            for(const DeathFloat* src = vbo->get_data();
                src != vbo->get_data() + vbo->get_size();
                src += 3)
            {
                m_true_bounds.extend(
                    *reinterpret_cast<const arc::lx::Vector3f*>(src)
                );
            }
        }

        // TODO: pass over volumetric data also

        // found no data - bail out
        if(m_true_bounds.isEmpty())
        {
            return;
        }
        m_empty = false;

        // determine the size of the octree
        arc::lx::Vector3f size = m_true_bounds.max() - m_true_bounds.min();
        m_root_size = std::max(std::max(size(0), size(1)), size(2));
        m_root_size /= MIN_SIZE;
        m_root_size = std::ceil(std::log2(m_root_size));
        m_depth = static_cast<DeathSize>(m_root_size);
        m_root_size = std::pow(2.0F, m_root_size) * MIN_SIZE;
        // m_depth = 2;

        // calculate the offset
        m_offset_trans = arc::lx::Vector3f(
            m_true_bounds.min()(0) + (size(0) / 2.0F),
            m_true_bounds.min()(1) + (size(1) / 2.0F),
            m_true_bounds.min()(2) + (size(2) / 2.0F)
        );
        m_offset *= arc::lx::translate_44f(m_offset_trans);
    }

    // builds the actual octant data of the octree
    void build_octants()
    {
        // build the root Octant
        m_root.reset(new Octant(m_root_size));

        for(death::Geometric* geometric : m_spatial->get_geometrics())
        {
            // get the primary VBO from the geometry
            death::VBO* vbo = geometric->get_vbo(0);
            // assume this is position data and traverse it find the spatial
            // bounds of this entity
            for(const DeathFloat* src = vbo->get_data();
                src != vbo->get_data() + vbo->get_size();
                src += 3)
            {
                set_data_for_octant(
                    *reinterpret_cast<const arc::lx::Vector3f*>(src)
                );
            }
        }
    }

    void set_data_for_octant(const arc::lx::Vector3f& position)
    {
        arc::lx::Vector3f offset = m_offset_trans;
        // explore until we reach the depth of the tree
        death::Octant* octant = m_root.get();
        for(DeathSize depth = 0; depth < m_depth; ++depth)
        {
            octant->branch(1);
            // find the next child
            int32_t x = -1;
            if(position(0) > offset(0))
            {
                x = 1;
            }
            int32_t y = -1;
            if(position(1) > offset(1))
            {
                y = 1;
            }
            int32_t z = -1;
            if(position(2) > offset(2))
            {
                z = 1;
            }

            // TODO:
            octant = octant->get_child(
                x > 0,
                y > 0,
                z > 0
            );
            // move the offset
            float half_size = octant->get_size() / 2.0F;
            offset(0) += x * half_size;
            offset(1) += y * half_size;
            offset(2) += z * half_size;
        }
        octant->set_data();
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

Octree::Octree(death::Spatial* spatial)
    : m_impl(new OctreeImpl(spatial))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

Octree::~Octree()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

bool Octree::is_empty() const
{
    return m_impl->is_empty();
}

const arc::lx::AABB3f& Octree::get_true_bounds() const
{
    return m_impl->get_true_bounds();
}

float Octree::get_root_size() const
{
    return m_impl->get_root_size();
}

const arc::lx::Matrix44f& Octree::get_offset() const
{
    return m_impl->get_offset();
}

death::GLOctree* Octree::get_debug_octree()
{
    return m_impl->get_debug_octree();
}

death::GLCells* Octree::get_debug_cells()
{
    return m_impl->get_debug_cells();
}

} // namespace death
