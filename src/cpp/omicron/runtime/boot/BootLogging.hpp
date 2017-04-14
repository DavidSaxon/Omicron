/*!
 * \file
 * \author David Saxon
 * \brief Provides the routines for starting and shutting down Omicron runtime
 *        logging.
 */
#ifndef OMICRON_RUNTIME_BOOT_LOGGING_HPP_
#define OMICRON_RUNTIME_BOOT_LOGGING_HPP_

namespace omi
{
namespace runtime
{
namespace boot
{

/*!
 * \brief Performs the startup subroutine for Omicron runtime logging.
 *
 * \return Whether startup completed successfully.
 */
bool startup_logging_subroutine();

/*!
 * \brief Performs the shutdown subroutine for Omicron runtime logging.
 *
 * \return Whether shutdown completed successfully.
 */
bool shutdown_logging_subroutine();

} // namespace boot
} // namespace runtime
} // namespace omi

#endif
