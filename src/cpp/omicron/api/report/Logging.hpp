/*!
 * \file
 * \author David Saxon
 * \brief Functionality related to logging through Omicron using ArcaneCore Log.
 */
#ifndef OMICRON_API_REPORT_LOGGING_HPP_
#define OMICRON_API_REPORT_LOGGING_HPP_

#include <arcanecore/log/LogHandler.hpp>

#include "omicron/api/report/ReportGlobals.hpp"


namespace omi
{
namespace report
{

//------------------------------------------------------------------------------
//                                    GLOBALS
//------------------------------------------------------------------------------

/*!
 * \brief The global log handler for all Omicron logging
 */
OMI_REPORT_GLOBAL_API extern arc::log::LogHandler log_handler;

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief Initialises the logging component of the report module.
 */
void logging_startup_routine();

} // namespace report
} // namespace omi

#endif
