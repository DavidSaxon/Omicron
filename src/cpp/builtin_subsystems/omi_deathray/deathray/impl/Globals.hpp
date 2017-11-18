/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 *
 * \brief Defines global variables and functions used by the DeathRay renderer
 *        internals.
 */
#ifndef DEATHRAY_IMPL_GLOBALS_HPP_
#define DEATHRAY_IMPL_GLOBALS_HPP_

#include "deathray/api/API.h"


//------------------------------------------------------------------------------
//                                     MACROS
//------------------------------------------------------------------------------

// TODO: REMOVE ME
// #define DEATH_DISABLE_LOGGING

/*!
 * \brief Macro that defines whether logging should be performed or not.
 */
#define DEATH_LOGGING 1
#if defined(DEATH_API_MODE_PRODUCTION) || defined(DEATH_DISABLE_LOGGING)
    #undef DEATH_LOGGING
#endif

#ifdef DEATH_LOGGING
    #include <arcanecore/log/Input.hpp>
    #include <arcanecore/log/LogHandler.hpp>
#endif


/*!
 * \brief Sends message stream to debug level logging (if logging is enabled).
 */
#define DEATH_LOG_DEBUG(msg)
#ifdef DEATH_LOGGING
    #undef DEATH_LOG_DEBUG
    #define DEATH_LOG_DEBUG(msg) \
        death::global::logger->debug << msg << std::endl;
#endif

/*!
 * \brief Sends message stream to info level logging (if logging is enabled).
 */
#define DEATH_LOG_INFO(msg)
#ifdef DEATH_LOGGING
    #undef DEATH_LOG_INFO
    #define DEATH_LOG_INFO(msg) \
        death::global::logger->info << msg << std::endl;
#endif

/*!
 * \brief Sends message stream to notice level logging (if logging is enabled).
 */
#define DEATH_LOG_NOTICE(msg)
#ifdef DEATH_LOGGING
    #undef DEATH_LOG_NOTICE
    #define DEATH_LOG_NOTICE(msg) \
        death::global::logger->notice << msg << std::endl;
#endif

/*!
 * \brief Sends message stream to warning level logging (if logging is enabled).
 */
#define DEATH_LOG_WARNING(msg)
#ifdef DEATH_LOGGING
    #undef DEATH_LOG_WARNING
    #define DEATH_LOG_WARNING(msg) \
        death::global::logger->warning << msg << std::endl;
#endif

/*!
 * \brief Sends message stream to error level logging (if logging is enabled).
 */
#define DEATH_LOG_ERROR(msg)
#ifdef DEATH_LOGGING
    #undef DEATH_LOG_ERROR
    #define DEATH_LOG_ERROR(msg) \
        death::global::logger->error << msg << std::endl;
#endif

/*!
 * \brief Sends message stream to critical level logging (if logging is
 *        enabled).
 */
#define DEATH_LOG_CRITICAL(msg)
#ifdef DEATH_LOGGING
    #undef DEATH_LOG_CRITICAL
    #define DEATH_LOG_CRITICAL(msg) \
        death::global::logger->critical << msg << std::endl;
#endif


namespace death
{

/*!
 * \brief Global variables and functions used by the DeathRay renderer
 *        internals.
 */
namespace global
{

//------------------------------------------------------------------------------
//                                   VARIABLES
//------------------------------------------------------------------------------

#ifdef DEATH_LOGGING

    /*!
     * \brief The log handler for DeathRay logging.
     */
    extern arc::log::LogHandler log_handler;

    /*!
     * \brief The logging input to be used by the DeathRay renderer
     */
    extern arc::log::Input* logger;

#endif

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief Sets up logging for DeathRay.
 */
void setup_logging();

} // namespace global
} // namespace death

#endif
