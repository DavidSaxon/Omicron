/*!
 * \file
 * \author David Saxon
 * \brief Logging for the Omicron Runtime using ArcaneLog.
 */
#ifndef OMICRON_RUNTIME_BASE_LOGGING_HPP_
#define OMICRON_RUNTIME_BASE_LOGGING_HPP_

#include <arcanelog/Input.hpp>
#include <arcanelog/Verbosity.hpp>

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

namespace arclog
{

class FileOutput;
class StdOutput;

} // namespace arclog

namespace omi
{
namespace runtime
{
/*!
 * \brief Logging components of the Omicron Runtime.
 */
namespace logging
{

//------------------------------------------------------------------------------
//                                    GLOBALS
//------------------------------------------------------------------------------

/*!
 * \brief The logging input to be used by the Omicron Runtime.
 */
extern arclog::Input* input;

} // namespace logging

/*!
 * \brief Convenience reference to the logger.
 *
 * Points to omi_::logging::input
 */
extern arclog::Input*& logger;

/*!
 * \brief Logging components of the Omicron Runtime.
 */
namespace logging
{

/*!
 * \brief The logging output to std::cout and std::cerr.
 */
extern arclog::StdOutput* std_output;
/*!
 * \brief The logging output for writing to the file system.
 */
extern arclog::FileOutput* file_output;

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief Initialises the logging from Metaengine data.
 */
void startup_routine();

/*!
 * \brief Returns the output stream that should be used to log critical messages
 *        to.
 *
 * This is useful when needing to log critical errors before knowing that
 * logging has been initialised.
 */
std::ostream& get_critical_stream();

} // namespace logging
} // namespace runtime
} // namespace omi

#endif
