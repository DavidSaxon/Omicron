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
