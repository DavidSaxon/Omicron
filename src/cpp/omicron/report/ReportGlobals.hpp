/*!
 * \file
 * \brief Globals for Omicron's report module.
 * \author David Saxon
 */
#ifndef OMICRON_REPORT_GLOBALS_HPP_
#define OMICRON_REPORT_GLOBALS_HPP_

#include <arcanecore/base/Preproc.hpp>
#include <arcanecore/io/sys/Path.hpp>

//------------------------------EXPORT DEFINITION-------------------------------
#ifdef ARC_OS_WINDOWS
    #ifdef OMI_REPORT_GLOBAL_EXPORT
        #define OMI_REPORT_GLOBAL_API __declspec(dllexport)
    #else
        #define OMI_REPORT_GLOBAL_API __declspec(dllimport)
    #endif
#else
    #define OMI_REPORT_GLOBAL_API
#endif
//------------------------------------------------------------------------------

namespace omi
{
namespace report
{

/*!
 * \brief Globals for Omicron's report module.
 */
namespace global
{

/*!
 * \brief The root directory where all Omicron's report meta programming is
 *        located within.
 */
extern const arc::io::sys::Path meta_root_dir;

/*!
 * \brief The directory where Omicron's logging meta programming is located
 *        within.
 */
extern const arc::io::sys::Path meta_logging_dir;

} // namespace global
} // namespace report
} // namespace omi

#endif
