/*!
 * \file
 * \author David Saxon
 * \brief Provides the main routines for startup and shutdown of the Omicron.
 */
#ifndef OMICRON_RUNTIME_BOOT_BOOTROUTINES_HPP_
#define OMICRON_RUNTIME_BOOT_BOOTROUTINES_HPP_

namespace omi_
{
namespace boot
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

// TODO: preflight boot (happens before splash screen hook) and same with
//       shutdown

/*!
 * \brief Performs the startup subroutines of Omicron.
 *
 * \return Whether startup completed successfully.
 */
bool startup_routine();

/*!
 * \brief Performs the shutdown subroutines of Omicron.
 *
 * \return Whether shutdown completed successfully.
 */
bool shutdown_routine();

} // namespace boot
} // namespace omi_

#endif
