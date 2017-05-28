/*!
 * \file
 * \brief Globals for Omicron's report module.
 * \author David Saxon
 */
#ifndef OMICRON_API_REPORT_GLOBALS_HPP_
#define OMICRON_API_REPORT_GLOBALS_HPP_

#include <arcanecore/io/sys/Path.hpp>


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
 * \brief The root directory where all Omicron's report configuration data is
 *        located within.
 */
extern const arc::io::sys::Path config_root_dir;

/*!
 * \brief The directory where Omicron's logging configuration data is located
 *        within.
 */
extern const arc::io::sys::Path config_logging_dir;

} // namespace global
} // namespace report
} // namespace omi

#endif
