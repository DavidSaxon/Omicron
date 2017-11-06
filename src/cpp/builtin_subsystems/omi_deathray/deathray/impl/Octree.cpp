/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/Octree.hpp"

#include <arcanecore/lx/Alignment.hpp>
#include <arcanecore/lx/Vector.hpp>

#include "deathray/impl/Geometric.hpp"
#include "deathray/impl/Spatial.hpp"
#include "deathray/impl/VBO.hpp"


namespace death
{

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

    // defines whether this Octree has no renderable data
    bool m_empty;
    // the bounding box which exactly contains the data of this spatial
    arc::lx::AABB3f m_true_bounds;

public:

    ARC_LX_ALIGNED_NEW;

    //--------------------------C O N S T R U C T O R---------------------------

    OctreeImpl(death::Spatial* spatial)
        : m_spatial(spatial)
        , m_empty  (true)
    {
        // build the octree data for the first time
        build();
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~OctreeImpl()
    {
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

private:

    //------------P R I V A T E    M E M B E R    F U N C T I O N S-------------

    // builds the data for this Octree
    void build()
    {
        // reset data
        m_empty = true;
        m_true_bounds.setEmpty();
        // TODO:

        // we must run a first pass to find the bounding boxes of the spatial
        build_bounding_boxes();

        // bail out if there's no renderable data
        if(m_empty)
        {
            return;
        }

        // TODO:
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

        // found some renderable data?
        if(!m_true_bounds.isEmpty())
        {
            m_empty = false;

            // determine the octree bounds for the data
            // TODO:
        }
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

} // namespace death
