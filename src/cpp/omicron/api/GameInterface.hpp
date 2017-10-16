/*!
 * \file
 * \brief Functionality for game implementations to register callbacks.
 * \author David Saxon
 */
#ifndef OMICRON_API_GAMECALLBACKS_HPP_
#define OMICRON_API_GAMECALLBACKS_HPP_

#include "omicron/api/API.hpp"


namespace omi
{

//------------------------------------------------------------------------------
//                                TYPE DEFINITIONS
//------------------------------------------------------------------------------

/*!
 * \brief The function signature for the symbol that returns the name of the
 *        game to the engine.
 */
typedef const char* (GameGetNameFunc)();

/*!
 * \brief The function signature for the symbol that returns the version of the
 *        game to the engine.
 */
typedef const char* (GameGetVersionFunc)();

/*!
 * \brief The function signature for a callback made to the game implementation
 *        at the time Omicron is started.
 */
typedef bool (GameStartupRoutineFunc)();

/*!
 * \brief The function signature for a callback made to the game implementation
 *        when the first frame of Omicron is rendered.
 */
typedef bool (GameFirstFrameRoutineFunc)();

/*!
 * \brief The function signature for a callback made to the game implementation
 *        at the time Omicron is shutdown.
 */
typedef bool (GameShutdownRoutineFunc)();


} // namespace omi

//------------------------------------------------------------------------------
//                                     MACROS
//------------------------------------------------------------------------------

/*!
 * \brief Macros that must be defined once and only once within a game's dso.
 *
 * \param name The name of the game.
 * \param version The version number string of the game.
 */
#define OMI_GAME_DEFINE(name, version)                                         \
    extern "C"                                                                 \
    {                                                                          \
    OMI_PLUGIN_EXPORT const char* OMI_GAME_get_name()                          \
    {                                                                          \
        return name;                                                           \
    }                                                                          \
    OMI_PLUGIN_EXPORT const char* OMI_GAME_get_version()                       \
    {                                                                          \
        return version;                                                        \
    }                                                                          \
    }

/*!
 * \brief Defines the game function that will be called during Omicron's start
 *        period.
 *
 * \warning This function will be called before the GL context has been
 *          initialised, so no GL operations may be performed during this
 *          function, instead for this use OMI_GAME_FIRSTFRAME_ROUTINE.
 *
 * \note This definition is optional but may only be used once within a game
 *       dso.
 *
 * This function should return ```true``` if it was successful and ```false```
 * otherwise.
 */
#define OMI_GAME_STARTUP_ROUTINE(function)                                     \
    extern "C"                                                                 \
    {                                                                          \
    OMI_PLUGIN_EXPORT bool OMI_GAME_startup_routine()                          \
    {                                                                          \
        return function();                                                     \
    }                                                                          \
    }

/*!
 * \brief Defines the game function that will be called at the start of
 *        Omicron's first frame once a valid GL context has been required.
 *
 * \note This definition is optional but may only be used once within a game
 *       dso.
 *
 * This function should return ```true``` if it was successful and ```false```
 * otherwise.
 */
#define OMI_GAME_FIRSTFRAME_ROUTINE(function)                                  \
    extern "C"                                                                 \
    {                                                                          \
    OMI_PLUGIN_EXPORT bool OMI_GAME_firstframe_routine()                       \
    {                                                                          \
        return function();                                                     \
    }                                                                          \
    }

/*!
 * \brief Defines the game function that will be called during Omicron's
 *        shutdown period.
 *
 * \note This definition is optional but may only be used once within a game
 *       dso.
 *
 * This function should return ```true``` if it was successful and ```false```
 * otherwise.
 */
#define OMI_GAME_SHUTDOWN_ROUTINE(function)                                    \
    extern "C"                                                                 \
    {                                                                          \
    OMI_PLUGIN_EXPORT bool OMI_GAME_shutdown_routine()                         \
    {                                                                          \
        return function();                                                     \
    }                                                                          \
    }

#endif
