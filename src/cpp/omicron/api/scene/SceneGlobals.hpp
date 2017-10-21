/*!
 * \file
 * \brief Globals for Omicron's scene module.
 * \author David Saxon
 */
#ifndef OMICRON_API_SCENE_GLOBALS_HPP_
#define OMICRON_API_SCENE_GLOBALS_HPP_

#include <arcanecore/log/Input.hpp>


namespace omi
{
namespace scene
{

/*!
 * \brief Globals for Omicron's scene module.
 */
namespace global
{

/*!
 * \brief The logging input to be used by the Omicron scene.
 */
extern arc::log::Input* logger;

} // namespace global
} // namespace scene
} // namespace omi

#endif
