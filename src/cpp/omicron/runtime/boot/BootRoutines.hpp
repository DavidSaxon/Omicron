/*!
 * \file
 * \author David Saxon
 * \brief Provides the main routines for startup and shutdown of the Omicron.
 */
#ifndef OMICRON_RUNTIME_BOOT_BOOTROUTINES_HPP_
#define OMICRON_RUNTIME_BOOT_BOOTROUTINES_HPP_

#include <iostream>


namespace omi
{
namespace runtime
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
 * \brief Performs setup of the first frame.
 *
 * This setup is performed with a valid GL context.
 */
bool firstframe_routine();

/*!
 * \brief Performs the shutdown subroutines of Omicron.
 *
 * \return Whether shutdown completed successfully.
 */
bool shutdown_routine();

/*!
 * \brief Returns the output stream that should be used to log critical messages
 *        to.
 *
 * This is useful when needing to log critical errors before knowing that
 * logging has been initialised.
 */
std::ostream& get_critical_stream();

} // namespace boot
} // namespace runtime
} // namespace omi

#endif
