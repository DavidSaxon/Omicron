/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/Spatial.hpp"

#include <new>

#include "deathray/impl/Geometric.hpp"


namespace death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class Spatial::SpatialImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the API handle to the spatial entity
    DeathSpatialHandle m_handle;

    // the set of geometric objects within the spatial entity
    std::unordered_set<death::Geometric*> m_geometrics;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    SpatialImpl(DeathSpatialHandle handle)
        : m_handle(handle)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~SpatialImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    const std::unordered_set<death::Geometric*> get_geometrics() const
    {
        return m_geometrics;
    }

    DeathError attach_geo(death::Geometric* geometric)
    {
        if(m_geometrics.find(geometric) != m_geometrics.end())
        {
            return kDeathErrorNoOperation;
        }

        m_geometrics.insert(geometric);
        return kDeathSuccess;
    }

    DeathError remove_geo(death::Geometric* geometric)
    {
        auto f_geometric = m_geometrics.find(geometric);
        if(f_geometric == m_geometrics.end())
        {
            return kDeathErrorNoOperation;
        }

        m_geometrics.erase(f_geometric);
        return kDeathSuccess;
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

Spatial::Spatial(DeathSpatialHandle handle)
    : m_impl(new SpatialImpl(handle))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

Spatial::~Spatial()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

const std::unordered_set<death::Geometric*> Spatial::get_geometrics() const
{
    return m_impl->get_geometrics();
}

DeathError Spatial::attach_geo(death::Geometric* geometric)
{
    return m_impl->attach_geo(geometric);
}

DeathError Spatial::remove_geo(death::Geometric* geometric)
{
    return m_impl->remove_geo(geometric);
}

} // namespace death

//------------------------------------------------------------------------------
//                               API IMPLEMENTATION
//------------------------------------------------------------------------------

DEATH_API_EXPORT DeathError death_spatial_gen(
        DeathSize count,
        DeathSpatialHandle* spatials)
{
    DeathSpatial* handles = nullptr;
    try
    {
        handles = new DeathSpatial[count];
        for(DeathSize i = 0; i < count; ++i)
        {
            handles[i].impl = new death::Spatial(handles + i);
        }
    }
    catch(std::bad_alloc&)
    {
        if(handles != nullptr)
        {
            for(DeathSize i = 0; i < count; ++i)
            {
                if(handles[i].impl != nullptr)
                {
                    delete handles[i].impl;
                }
            }
            delete[] handles;
        }
        return kDeathErrorOutOfMemory;
    }

    *spatials = handles;
    return kDeathSuccess;
}

DEATH_API_EXPORT DeathError death_spatial_delete(
        DeathSize count,
        DeathSpatialHandle* spatials)
{
    if(spatials == nullptr)
    {
        return kDeathErrorNullHandle;
    }

    for(DeathSize i = 0; i < count; ++i)
    {
        delete spatials[i]->impl;
    }
    delete[] *spatials;
    *spatials = nullptr;

    return kDeathSuccess;
}

DEATH_API_EXPORT DeathError death_spatial_attach_geo(
        DeathSpatialHandle spatial,
        DeathGeometricHandle geometric)
{
    if(spatial == nullptr || geometric == nullptr)
    {
        return kDeathErrorNullHandle;
    }

    return spatial->impl->attach_geo(geometric->impl);
}

DEATH_API_EXPORT DeathError death_spatial_remove_geo(
        DeathSpatialHandle spatial,
        DeathGeometricHandle geometric)
{
    if(spatial == nullptr || geometric == nullptr)
    {
        return kDeathErrorNullHandle;
    }

    return spatial->impl->remove_geo(geometric->impl);
}
