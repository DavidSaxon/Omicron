/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/Geometric.hpp"

#include <new>
#include <unordered_map>

#include "deathray/impl/VBO.hpp"
#include "deathray/impl/debug/GLGeometry.hpp"


namespace death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class Geometric::GeometricImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //---------------------T Y P E    D E F I N I T I O N S---------------------

    typedef std::unordered_map<DeathUInt32, death::VBO*> VBOMap;

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the API handle to the geometric object
    DeathGeometricHandle m_handle;

    // mapping from indices to the associated VBOs
    std::unordered_map<DeathUInt32, death::VBO*> m_vbos;

    // the debug geometry
    death::GLGeometry* m_debug_geo;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    GeometricImpl(DeathGeometricHandle handle)
        : m_handle   (handle)
        , m_debug_geo(nullptr)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~GeometricImpl()
    {
        // TODO: need a graphics state queue for this
        if(m_debug_geo != nullptr)
        {
            delete m_debug_geo;
            m_debug_geo = nullptr;
        }
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    death::VBO* get_vbo(DeathUInt32 index)
    {
        auto f_vbo = m_vbos.find(index);
        if(f_vbo != m_vbos.end())
        {
            return f_vbo->second;
        }
        return nullptr;
    }

    DeathError attach_vbo(DeathUInt32 index, death::VBO* vbo)
    {
        // vbo already attached?
        auto f_vbo = m_vbos.find(index);
        if(f_vbo != m_vbos.end())
        {
            if(f_vbo->second == vbo)
            {
                return kDeathErrorNoOperation;
            }
        }

        m_vbos.insert(std::make_pair(index, vbo));
        return kDeathSuccess;
    }

    DeathError remove_vbo(death::VBO* vbo)
    {
        // search for the vbo
        VBOMap::iterator r = m_vbos.end();
        for(VBOMap::iterator i = m_vbos.begin(); i != m_vbos.end(); ++i)
        {
            if(i->second == vbo)
            {
                r = i;
                break;
            }
        }
        if(r == m_vbos.end())
        {
            return kDeathErrorNoOperation;
        }

        m_vbos.erase(r);
        return kDeathSuccess;
    }

    death::GLGeometry* get_debug_geo()
    {
        // already exists?
        if(m_debug_geo != nullptr)
        {
            return m_debug_geo;
        }
        // no valid data
        death::VBO* vbo = get_vbo(0);
        if(vbo == nullptr || vbo->get_tuple_size() != 3)
        {
            return nullptr;
        }

        // build
        m_debug_geo = new GLGeometry(vbo->get_data(), vbo->get_size());
        return m_debug_geo;
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

Geometric::Geometric(DeathGeometricHandle handle)
    : m_impl(new GeometricImpl(handle))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

Geometric::~Geometric()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

death::VBO* Geometric::get_vbo(DeathUInt32 index)
{
    return m_impl->get_vbo(index);
}

DeathError Geometric::attach_vbo(DeathUInt32 index, death::VBO* vbo)
{
    return m_impl->attach_vbo(index, vbo);
}

DeathError Geometric::remove_vbo(death::VBO* vbo)
{
    return m_impl->remove_vbo(vbo);
}

death::GLGeometry* Geometric::get_debug_geo()
{
    return m_impl->get_debug_geo();
}

} // namespace death

//------------------------------------------------------------------------------
//                               API IMPLEMENTATION
//------------------------------------------------------------------------------

DEATH_API_EXPORT DeathError death_geo_gen(
        DeathSize count,
        DeathGeometricHandle* geometrics)
{
    DeathGeometric* handles = nullptr;
    try
    {
        handles = new DeathGeometric[count];
        for(DeathSize i = 0; i < count; ++i)
        {
            handles[i].impl = new death::Geometric(handles + i);
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

    *geometrics = handles;
    return kDeathSuccess;
}

DEATH_API_EXPORT DeathError death_geo_delete(
        DeathSize count,
        DeathGeometricHandle* geometrics)
{
    if(geometrics == nullptr)
    {
        return kDeathErrorNullHandle;
    }

    for(DeathSize i = 0; i < count; ++i)
    {
        delete geometrics[i]->impl;
    }
    delete[] *geometrics;
    *geometrics = nullptr;

    return kDeathSuccess;
}

DEATH_API_EXPORT DeathError death_geo_attach_vbo(
        DeathGeometricHandle geometric,
        DeathUInt32 index,
        DeathVBOHandle vbo)
{
    if(geometric == nullptr || vbo == nullptr)
    {
        return kDeathErrorNullHandle;
    }

    return geometric->impl->attach_vbo(index, vbo->impl);
}

DEATH_API_EXPORT DeathError death_geo_remove_vbo(
        DeathGeometricHandle geometric,
        DeathVBOHandle vbo)
{
    if(geometric == nullptr || vbo == nullptr)
    {
        return kDeathErrorNullHandle;
    }

    return geometric->impl->remove_vbo(vbo->impl);
}
