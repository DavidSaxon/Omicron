/*!
 * \file
 * \brief Globals for Omicron's asset module.
 * \author David Saxon
 */
#ifndef OMICRON_API_ASSET_GLOBALS_HPP_
#define OMICRON_API_ASSET_GLOBALS_HPP_

#include <arcanecore/io/sys/Path.hpp>
#include <arcanecore/log/Input.hpp>


namespace omi
{
namespace asset
{

/*!
 * \brief Globals for Omicron's asset module.
 */
namespace global
{

/*!
 * \brief The logging input to be used by the Omicron asset system.
 */
extern arc::log::Input* logger;

/*!
 * \brief The root directory where all Omicron's asset configuration data is
 *        located within.
 */
extern const arc::io::sys::Path config_root_dir;

} // namespace global
} // namespace asset
} // namespace omi

#endif
