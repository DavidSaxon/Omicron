/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/VBO.hpp"

#include <cstring>
#include <limits>
#include <new>

#include "deathray/impl/util/DataUtil.hpp"


namespace death
{

//------------------------------------------------------------------------------
//                                    GLOBALS
//------------------------------------------------------------------------------

namespace
{

static const DeathFloat MIN_VALUE = std::numeric_limits<DeathFloat>::min();
static const DeathFloat MAX_VALUE = std::numeric_limits<DeathFloat>::max();

} // namespace anonymous

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class VBO::VBOImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the API handle to the VBO
    DeathVBOHandle m_handle;

    DeathSize m_size;
    DeathSize m_tuple_size;
    DeathFloat* m_data;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    VBOImpl(DeathVBOHandle handle)
        : m_handle    (handle)
        , m_size      (0)
        , m_tuple_size(0)
        , m_data      (nullptr)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~VBOImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    DeathSize get_size() const
    {
        return m_size;
    }

    DeathSize get_tuple_size() const
    {
        return m_tuple_size;
    }

    const DeathFloat* get_data() const
    {
        return m_data;
    }

    DeathError set_data(
            DeathDataType data_type,
            DeathSize size,
            DeathSize tuple_size,
            const void* data)
    {
        release();

        // ensure tuple size is a factor of the data size
        if(size % tuple_size != 0)
        {
            return kDeathErrorTupleSizeNotAFactor;
        }

        // allocate
        try
        {
            m_data = new DeathFloat[size];
        }
        catch(std::bad_alloc&)
        {
            return kDeathErrorOutOfMemory;
        }

        // cast and copy based on the datatype
        switch(data_type)
        {
            case kDeathByte:
                copy_convert<DeathByte>(data, size);
                break;
            case kDeathUByte:
                copy_convert<DeathByte>(data, size);
                break;
            case kDeathInt16:
                copy_convert<DeathByte>(data, size);
                break;
            case kDeathUInt16:
                copy_convert<DeathByte>(data, size);
                break;
            case kDeathInt32:
                copy_convert_clamp<DeathByte>(data, size);
                break;
            case kDeathUInt32:
                copy_convert_unsigned_clamp<DeathByte>(data, size);
                break;
            case kDeathInt64:
                copy_convert_clamp<DeathByte>(data, size);
                break;
            case kDeathUInt64:
                copy_convert_unsigned_clamp<DeathByte>(data, size);
                break;
            case kDeathFloat:
                // just do a memcpy for floats for performance
                std::memcpy(m_data, data, size * sizeof(DeathFloat));
                break;
            case kDeathDouble:
                copy_convert_clamp<DeathByte>(data, size);
                break;
            default:
                release();
                return kDeathErrorInvalidEnumValue;
        }

        // finialise
        m_size = size;
        m_tuple_size = tuple_size;
        return kDeathSuccess;
    }

    void release()
    {
        if(m_data != nullptr)
        {
            delete[] m_data;
            m_data = nullptr;
            m_size = 0;
        }
    }

private:

    //------------P R I V A T E    M E M B E R    F U N C T I O N S-------------

    // reads data of the given type and stores it within this object without
    // performing any clamping
    template<typename T_DataType>
    void copy_convert(const void* data, DeathSize size)
    {
        const T_DataType* src = reinterpret_cast<const T_DataType*>(data);
        DeathFloat* dest = m_data;
        for(; src != src + size; ++src)
        {
            *dest = static_cast<DeathFloat>(*src);
            ++dest;
        }
    }

    // reads data of the given type and stores it within this object and
    // perform positive limits clamping
    template<typename T_DataType>
    void copy_convert_unsigned_clamp(const void* data, DeathSize size)
    {
        const T_DataType* src = reinterpret_cast<const T_DataType*>(data);
        DeathFloat* dest = m_data;
        for(; src != src + size; ++src)
        {
            if((*src) > static_cast<T_DataType>(MAX_VALUE))
            {
                *dest = MAX_VALUE;
            }
            else
            {
                *dest = static_cast<DeathFloat>(*src);
            }
            ++dest;
        }
    }

    // reads data of the given type and stores it within this object and
    // perform  clamping
    template<typename T_DataType>
    void copy_convert_clamp(const void* data, DeathSize size)
    {
        const T_DataType* src = reinterpret_cast<const T_DataType*>(data);
        DeathFloat* dest = m_data;
        for(; src != src + size; ++src)
        {
            if((*src) < static_cast<T_DataType>(MIN_VALUE))
            {
                *dest = MIN_VALUE;
            }
            else if((*src) > static_cast<T_DataType>(MAX_VALUE))
            {
                *dest = MAX_VALUE;
            }
            else
            {
                *dest = static_cast<DeathFloat>(*src);
            }
            ++dest;
        }
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

VBO::VBO(DeathVBOHandle handle)
    : m_impl(new VBOImpl(handle))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

VBO::~VBO()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

DeathSize VBO::get_size() const
{
    return m_impl->get_size();
}

DeathSize VBO::get_tuple_size() const
{
    return m_impl->get_tuple_size();
}

const DeathFloat* VBO::get_data() const
{
    return m_impl->get_data();
}

DeathError VBO::set_data(
        DeathDataType data_type,
        DeathSize size,
        DeathSize tuple_size,
        const void* data)
{
    return m_impl->set_data(data_type, size, tuple_size, data);
}

void VBO::release()
{
    m_impl->release();
}

} // namespace death

//------------------------------------------------------------------------------
//                               API IMPLEMENTATION
//------------------------------------------------------------------------------

DEATH_API_EXPORT DeathError death_vbo_gen(
        DeathSize count,
        DeathVBOHandle* vbos)
{
    DeathVBO* handles = nullptr;
    try
    {
        handles = new DeathVBO[count];
        for(DeathSize i = 0; i < count; ++i)
        {
            handles[i].impl = new death::VBO(handles + i);
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

    *vbos = handles;
    return kDeathSuccess;
}

DEATH_API_EXPORT DeathError death_vbo_delete(
        DeathSize count,
        DeathVBOHandle* vbos)
{
    if(vbos == nullptr)
    {
        return kDeathErrorNullHandle;
    }

    for(DeathSize i = 0; i < count; ++i)
    {
        delete vbos[i]->impl;
    }
    delete[] *vbos;
    *vbos = nullptr;

    return kDeathSuccess;
}

DEATH_API_EXPORT DeathError death_vbo_set_data(
        DeathVBOHandle vbo,
        DeathDataType data_type,
        DeathSize size,
        DeathSize tuple_size,
        const void* data)
{
    if(vbo == nullptr)
    {
        return kDeathErrorNullHandle;
    }

    return vbo->impl->set_data(data_type, size, tuple_size, data);
}
