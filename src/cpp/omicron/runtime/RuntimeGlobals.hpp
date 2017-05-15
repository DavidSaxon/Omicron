/*!
 * \file
 * \author David Saxon
 * \brief Globals for the Omicron runtime.
 */
#ifndef OMICRON_RUNTIME_GLOBALS_HPP_
#define OMICRON_RUNTIME_GLOBALS_HPP_

#include <arcanecore/io/sys/Path.hpp>

#include <arcanecore/log/Input.hpp>

namespace omi
{
namespace runtime
{
/*!
 * \brief Global objects for the Omicron runtime.
 */
namespace global
{

/*!
 * \brief The logging input to be used by the Omicron Runtime.
 */
extern arc::log::Input* logger;

/*!
 * \brief The root directory where all Omicron's runtime meta programming is
 *        located within.
 */
extern const arc::io::sys::Path config_root_dir;

} // namespace global
} // namespace runtime
} // namespace omi

#endif
