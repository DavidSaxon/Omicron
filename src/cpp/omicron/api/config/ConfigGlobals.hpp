/*!
 * \file
 * \brief Globals relating to configuration data through ArcaneCore Config in
 *        Omicron.
 * \author David Saxon
 */
#ifndef OMICRON_API_CONFIG_GLOBALS_HPP_
#define OMICRON_API_CONFIG_GLOBALS_HPP_

#include <arcanecore/base/Preproc.hpp>
#include <arcanecore/io/sys/Path.hpp>

#include "omicron/api/API.hpp"


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
OMI_API_EXPORT extern const arc::io::sys::Path root_dir;

} // namespace global
} // namespace config
} // namespace omi

#endif
