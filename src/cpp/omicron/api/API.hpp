/*!
 * \file
 * \brief Globals definitions for the Omicron API.
 * \author David Saxon
 */
#ifndef OMICRON_API_HPP_
#define OMICRON_API_HPP_

#include <arcanecore/base/Preproc.hpp>


namespace omi
{

// TODO: sort out when there's a proper build system
#define OMI_API_MODE_DEBUG

//-------------------------------COMPILATION MODE-------------------------------
#if !defined(OMI_API_MODE_DEBUG) && !defined(OMI_API_MODE_DEVELOPER)
#define OMI_API_MODE_PRODUCTION
#endif

//------------------------------EXPORT DEFINITION-------------------------------
#ifdef ARC_OS_WINDOWS
    #ifdef OMI_API_GLOBAL_EXPORT
        #define OMI_API_GLOBAL __declspec(dllexport)
    #else
        #define OMI_API_GLOBAL __declspec(dllimport)
    #endif
#else
    #define OMI_API_GLOBAL
#endif
//------------------------------------------------------------------------------

} // namespace omi

#endif
