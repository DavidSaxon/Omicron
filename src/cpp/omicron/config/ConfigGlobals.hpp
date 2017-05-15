/*!
 * \file
 * \brief Globals relating to configuration data through ArcaneCore Config in
 *        Omicron.
 * \author David Saxon
 */
#ifndef OMICRON_META_GLOBALS_HPP_
#define OMICRON_META_GLOBALS_HPP_

#include <arcanecore/base/Preproc.hpp>
#include <arcanecore/io/sys/Path.hpp>

//------------------------------EXPORT DEFINITION-------------------------------
#ifdef ARC_OS_WINDOWS
    #ifdef OMI_CONFIG_GLOBAL_EXPORT
        #define OMI_CONFIG_GLOBAL_API __declspec(dllexport)
    #else
        #define OMI_CONFIG_GLOBAL_API __declspec(dllimport)
    #endif
#else
    #define OMI_CONFIG_GLOBAL_API
#endif
//------------------------------------------------------------------------------

namespace omi
{
namespace config
{

/*!
 * \brief Globals relating to configuration data though ArcaneCore Config in
 *        Omicron.
 */
namespace global
{

/*!
 * \brief The root directory where all Omicron config data is located within.
 */
OMI_CONFIG_GLOBAL_API extern const arc::io::sys::Path root_dir;

} // namespace global
} // namespace config
} // namespace omi

#endif
