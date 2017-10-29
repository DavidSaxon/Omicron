/*!
 * \file
 * \brief Globals definitions for the Omicron API.
 * \author David Saxon
 */
#ifndef OMICRON_API_HPP_
#define OMICRON_API_HPP_

#include <cassert>

#include <arcanecore/base/Preproc.hpp>
#include <arcanecore/base/lang/Restrictors.hpp>


namespace omi
{

// TODO: sort out when there's a proper build system
#define OMI_API_MODE_DEBUG

//-------------------------------COMPILATION MODE-------------------------------
#if !defined(OMI_API_MODE_DEBUG) && !defined(OMI_API_MODE_DEVELOPER)
#define OMI_API_MODE_PRODUCTION
#endif
//------------------------------------------------------------------------------

//----------------------------API EXPORT DEFINITION-----------------------------
#ifdef ARC_OS_WINDOWS
    #ifdef OMI_API_ENABLE_EXPORT
        #define OMI_API_EXPORT __declspec(dllexport)
    #else
        #define OMI_API_EXPORT __declspec(dllimport)
    #endif
#else
    #define OMI_API_EXPORT
#endif
//------------------------------------------------------------------------------

//---------------------------PLUGIN EXPORT DEFINITION---------------------------
#ifdef ARC_OS_WINDOWS
    #ifdef OMI_PLUGIN_ENABLE_EXPORT
        #define OMI_PLUGIN_EXPORT __declspec(dllexport)
    #else
        #define OMI_PLUGIN_EXPORT __declspec(dllimport)
    #endif
#else
    #define OMI_PLUGIN_EXPORT
#endif
//------------------------------------------------------------------------------

//------------------------------SUBSYSTEM EXCHANGE------------------------------

/*!
 * \brief Function signature for getting the version of a subsystem.
 */
typedef const char* (SubsytemGetVersionFunc)();

/*!
 * \brief Function signature for a subsystem object create function passed via
 *        a C interface.
 */
typedef void* (SubsystemObjectCreateFunc)();

/*!
 * \brief Function signature for a subsystem object destroy function passed via
 *        a C interface.
 */
typedef void (SubsystemObjectDestroyFunc)(void*);


/*!
 * \brief Simple object that wraps a subsystem factory and destroy function
 *        and ensures that if the object has been constructed it is deleted at
 *        the same time as this object.
 */
template <typename T_ObjectType>
class SubsytemObject
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    SubsytemObject(
            SubsystemObjectCreateFunc* factory_func,
            SubsystemObjectDestroyFunc* destroy_func)
        : m_factory_func(factory_func)
        , m_destroy_func(destroy_func)
        , m_object      (nullptr)
    {
        assert(factory_func != nullptr);
        assert(destroy_func != nullptr);
    }

    virtual ~SubsytemObject()
    {
        if(m_object != nullptr)
        {
            m_destroy_func(m_object);
            m_object = nullptr;
        }
    }

    T_ObjectType* get()
    {
        if(m_object == nullptr)
        {
            m_object = static_cast<T_ObjectType*>(m_factory_func());
        }
        return m_object;
    }

private:

    SubsystemObjectCreateFunc* m_factory_func;
    SubsystemObjectDestroyFunc* m_destroy_func;
    T_ObjectType* m_object;
};

//------------------------------------------------------------------------------

} // namespace omi

#endif
