/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/Octant.hpp"

#include <vector>

#include <arcanecore/crypt/hash/FNV.hpp>


namespace death
{

//------------------------------------------------------------------------------
//                                    GLOBALS
//------------------------------------------------------------------------------

namespace
{

static arc::uint32 g_id = 0;

} // namespace anonymous

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class Octant::OctantImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the world space size of this octant
    DeathFloat m_size;
    // the "unique" id of this octant
    Octant::Id m_id;

    // whether this octant contains any renderable data
    bool m_data;

    // the child octants (empty if this is a leaf octant)
    std::vector<Octant*> m_children;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    OctantImpl(DeathFloat size)
        : m_size(size)
        , m_data(false)
    {
        m_id.integer = g_id++;
        // TODO: REMOVE THIS
        // hash it real quick
        m_id.integer = arc::crypt::hash::fnv1a_32(&m_id, sizeof(Octant::Id));
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~OctantImpl()
    {
        // clean up
        for(Octant* child : m_children)
        {
            delete child;
        }
        m_children.clear();
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    float get_size() const
    {
        return m_size;
    }

    Id get_id() const
    {
        return m_id;
    }

    bool has_data() const
    {
        return m_data;
    }

    void set_data()
    {
        m_data = true;
    }

    bool is_leaf() const
    {
        return m_children.empty();
    }

    void branch(DeathSize depth)
    {
        // bail out with 0 depth
        if(depth == 0)
        {
            return;
        }

        // create children at this level
        if(m_children.empty())
        {
            m_children.reserve(8);
            for(std::size_t i = 0; i < 8; ++i)
            {
                m_children.push_back(new Octant(m_size / 2.0F));
            }
        }

        // branch children?
        DeathSize next_depth = depth - 1;
        if(next_depth > 0)
        {
            for(Octant* child : m_children)
            {
                child->branch(next_depth);
            }
        }
    }

    Octant* get_child(bool x, bool y, bool z)
    {
        std::size_t index = x + (y * 2) + (z * 4);
        if(index >= m_children.size())
        {
            return nullptr;
        }

        return m_children[index];
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

Octant::Octant(DeathFloat size)
    : m_impl(new OctantImpl(size))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

Octant::~Octant()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

float Octant::get_size() const
{
    return m_impl->get_size();
}

Octant::Id Octant::get_id() const
{
    return m_impl->get_id();
}

bool Octant::has_data() const
{
    return m_impl->has_data();
}

void Octant::set_data()
{
    m_impl->set_data();
}

bool Octant::is_leaf() const
{
    return m_impl->is_leaf();
}

void Octant::branch(DeathSize depth)
{
    m_impl->branch(depth);
}

Octant* Octant::get_child(bool x, bool y, bool z)
{
    return m_impl->get_child(x, y, z);
}

} // namespace death
