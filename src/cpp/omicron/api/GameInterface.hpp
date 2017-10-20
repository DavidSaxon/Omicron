/*!
 * \file
 * \brief Functionality for game implementations to register callbacks.
 * \author David Saxon
 */
#ifndef OMICRON_API_GAMECALLBACKS_HPP_
#define OMICRON_API_GAMECALLBACKS_HPP_

#include <vector>

#include <arcanecore/base/str/UTF8String.hpp>

#include "omicron/api/API.hpp"
#include "omicron/api/common/attribute/Attribute.hpp"


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

/*!
 * \brief The function signature used to get the number of different entity
 *        types that have been registered in Omicron.
 */
typedef uint64_t (GameEntityTypeCountFunc)();

/*!
 * \brief The function signature used to get the names the of different entity
 *        types that have been registered in Omicron.
 */
typedef const char** (GameEntityTypeNamesFunc)();

/*!
 * \brief The function signature used to construct a game entity.
 */
typedef void* (GameEntityFactory)(
        const arc::str::UTF8String&,
        const omi::Attribute&);

/*!
 * \brief The function signature used to destroy a game entity.
 */
typedef void (GameEntityDestroy)(void*);


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
    std::vector<const char*> OMI_GAME_entity_reg;                              \
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
    OMI_PLUGIN_EXPORT uint64_t OMI_GAME_entity_type_count()                    \
    {                                                                          \
        return static_cast<uint64_t>(OMI_GAME_entity_reg.size());              \
    }                                                                          \
    OMI_PLUGIN_EXPORT const char** OMI_GAME_entity_type_names()                \
    {                                                                          \
        return &OMI_GAME_entity_reg[0];                                        \
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

/*!
 * \brief Registers an entity for use within the game engine.
 *
 * \note This macro must be used outside of any namespaces.
 *
 * \param EntityType The class name of the entity implementation to be
 *                   registered.
 * \param id The string identifier that will be used to construct new instances
 *           of this entity type.
 */
#define OMI_GAME_REGISTER_ENTITY(EntityType, id)                               \
    extern std::vector<const char*> OMI_GAME_entity_reg;                       \
    namespace                                                                  \
    {                                                                          \
    class id##Register                                                         \
    {                                                                          \
    public:                                                                    \
        id##Register()                                                         \
        {                                                                      \
            OMI_GAME_entity_reg.push_back(#id);                                \
        }                                                                      \
    };                                                                         \
    static id##Register id##_register;                                         \
    }                                                                          \
    extern "C"                                                                 \
    {                                                                          \
    OMI_PLUGIN_EXPORT void* id##_factory(                                      \
            const arc::str::UTF8String& name,                                  \
            const omi::Attribute& data)                                        \
    {                                                                          \
        return new EntityType(name, data);                                     \
    }                                                                          \
    OMI_PLUGIN_EXPORT void id##_destroy(void* e)                               \
    {                                                                          \
        delete static_cast<EntityType*>(e);                                    \
    }                                                                          \
    }

#endif
