/*!
 * \file
 * \author David Saxon
 * \brief Functionality for starting up and shutting down the Omicron reporting
 *        module.
 */
#ifndef OMICRON_REPORT_BOOT_HPP_
#define OMICRON_REPORT_BOOT_HPP_
// TODO: hide from public API

#include "omicron/report/ReportGlobals.hpp"

namespace omi
{
namespace report
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief Initializes the report module.
 */
OMI_REPORT_GLOBAL_API void startup_routine();

/*!
 * \brief Shutsdown the report module.
 */
OMI_REPORT_GLOBAL_API void shutdown_routine();

} // namespace report
} // namespace omi

#endif
