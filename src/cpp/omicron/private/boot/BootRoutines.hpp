/*!
 * \file
 * \author David Saxon
 * \brief Provides the main routines for startup and shutdown of the Omicron.
 */
#ifndef OMICRON_PRIVATE_BOOT_BOOTROUTINES_HPP_
#define OMICRON_PRIVATE_BOOT_BOOTROUTINES_HPP_

#include <iostream>

namespace omi_
{
namespace boot
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

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

/*!
 * \brief Returns the output stream that should be used to log critical messages
 *        to.
 *
 * This is useful for needing to log critical errors before we know that logging
 * has been initialised.
 */
std::ostream* get_critical_stream();

} // namespace boot
} // namespace omi_

#endif
