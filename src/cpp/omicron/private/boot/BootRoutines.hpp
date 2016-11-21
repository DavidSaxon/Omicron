/*!
 * \file
 * \author David Saxon
 * \brief Provides the main routines for startup and shutdown of the Omicron.
 */
#ifndef OMICRON_PRIVATE_BOOT_BOOTROUTINES_HPP_
#define OMICRON_PRIVATE_BOOT_BOOTROUTINES_HPP_

namespace omi
{
namespace boot
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief Performs the startup subroutines of Omicron.
 */
void startup_routine();

/*!
 * \brief Performs the shutdown subroutines of Omicron.
 */
void shutdown_routine();

} // namespace boot
} // namespace omi

#endif
