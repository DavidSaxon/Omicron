/*!
 * \file
 * \brief Globals for the Omicron Runtime's meta programming.
 * \author David Saxon
 */
#ifndef OMICRON_RUNTIME_META_GLOBALS_HPP_
#define OMICRON_RUNTIME_META_GLOBALS_HPP_

#include <arcanecore/io/sys/Path.hpp>

namespace omi_
{
namespace meta
{
/*!
 * \brief Globals for the Omicron Runtime's meta programming.
 */
namespace global
{

/*!
 * \param The path to the Omicron Runtime's meta programming directory.
 */
extern const arc::io::sys::Path runtime_dir;

} // namespace global
} // namespace meta
} // namespace omi_

#endif
