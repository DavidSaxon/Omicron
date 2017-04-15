/*!
 * \file
 * \brief Globals for the meta programming in Omicron.
 * \author David Saxon
 */
#ifndef OMICRON_META_GLOBALS_HPP_
#define OMICRON_META_GLOBALS_HPP_

#include <arcanecore/base/Preproc.hpp>
#include <arcanecore/io/sys/Path.hpp>

//------------------------------EXPORT DEFINITION-------------------------------
#ifdef ARC_OS_WINDOWS
    #ifdef OMI_META_GLOBAL_EXPORT
        #define OMI_META_GLOBAL_API __declspec(dllexport)
    #else
        #define OMI_META_GLOBAL_API __declspec(dllimport)
    #endif
#else
    #define OMI_META_GLOBAL_API
#endif
//------------------------------------------------------------------------------

namespace omi
{
namespace meta
{

/*!
 * \brief Globals for meta programming in Omicron.
 */
namespace global
{

/*!
 * \brief The root directory where all Omicron meta programming is located
 *        within.
 */
OMI_META_GLOBAL_API extern const arc::io::sys::Path root_dir;

} // namespace global
} // namespace meta
} // namespace omi

#endif
