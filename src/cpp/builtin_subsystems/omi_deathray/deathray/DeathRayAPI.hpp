/*!
 * \file
 * \brief Globals definitions for the DeathRay API.
 * \author David Saxon
 */
#ifndef DEATHRAY_API_HPP_
#define DEATHRAY_API_HPP_

#include <arcanecore/base/Preproc.hpp>


namespace death
{

// TODO: sort out when there's a proper build system
#define DEATH_API_MODE_DEBUG

//-------------------------------COMPILATION MODE-------------------------------
#if !defined(DEATH_API_MODE_DEBUG) && !defined(DEATH_API_MODE_DEVELOPMENT)
#define DEATH_API_MODE_PRODUCTION
#endif
//------------------------------------------------------------------------------

//----------------------------API EXPORT DEFINITION-----------------------------
#ifdef ARC_OS_WINDOWS
    #ifdef DEATH_API_ENABLE_EXPORT
        #define DEATH_API_EXPORT __declspec(dllexport)
    #else
        #define DEATH_API_EXPORT __declspec(dllimport)
    #endif
#else
    #define DEATH_API_EXPORT
#endif
//------------------------------------------------------------------------------

} // namespace death

#endif
