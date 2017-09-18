/*!
 * \file
 * \brief Globals for Omicron's resource module.
 * \author David Saxon
 */
#ifndef OMICRON_API_RES_GLOBALS_HPP_
#define OMICRON_API_RES_GLOBALS_HPP_

#include <arcanecore/io/sys/Path.hpp>
#include <arcanecore/log/Input.hpp>

namespace omi
{
namespace res
{

/*!
 * \brief Globals for Omicron's resource module.
 */
namespace global
{

/*!
 * \brief The logging input to be used by the Omicron resource system.
 */
extern arc::log::Input* logger;

/*!
 * \brief The root directory where all Omicron's resource configuration data is
 *        located within.
 */
extern const arc::io::sys::Path config_root_dir;

} // namespace global
} // namespace res
} // namespace omi

#endif
