/*!
 * \file
 * \author David Saxon
 * \brief Functionality related to logging through Omicron using ArcaneLog.
 */
#ifndef OMICRON_REPORT_LOGGING_HPP_
#define OMICRON_REPORT_LOGGING_HPP_

#include <arcanelog/LogHandler.hpp>

#include "omicron/report/ReportGlobals.hpp"

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
OMI_REPORT_GLOBAL_API extern arclog::LogHandler log_handler;

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
