/*!
 * \file
 * \brief Globals definitions for the DeathRay API.
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_API_H_
#define DEATHRAY_API_H_

#include <inttypes.h>
#include <stddef.h>


// TODO: sort out when there's a proper build system
#define DEATH_API_MODE_DEBUG

//-------------------------------COMPILATION MODE-------------------------------
#if !defined(DEATH_API_MODE_DEBUG) && !defined(DEATH_API_MODE_DEVELOPMENT)
#define DEATH_API_MODE_PRODUCTION
#endif
//------------------------------------------------------------------------------

//-----------------------------------PLATFORM-----------------------------------
#ifdef _WIN32
    #define DEATH_API_OS_WINDOWS
#endif

#ifdef __APPLE__
    #define DEATH_API_OS_MAC
#endif

#ifdef __linux
    #define DEATH_API_OS_LINUX
#endif

#if defined(__APPLE__) || defined(__linux) || !defined(__unix)
    #define DEATH_API_OS_UNIX
#endif

#if !defined(DEATH_API_OS_WINDOWS) || !defined(DEATH_API_OS_UNIX)
    #undef DEATH_API_OS_UNKNOWN
#endif

//----------------------------------NO MANGLE-----------------------------------
#define DEATH_API_NO_MANGLE_BEGIN
#ifdef __cplusplus
    #undef DEATH_API_NO_MANGLE_BEGIN
    #define DEATH_API_NO_MANGLE_BEGIN extern "C" {
#endif

#define DEATH_API_NO_MANGLE_END
#ifdef __cplusplus
    #undef DEATH_API_NO_MANGLE_END
    #define DEATH_API_NO_MANGLE_END }
#endif
//------------------------------------------------------------------------------

//----------------------------API EXPORT DEFINITION-----------------------------
#ifdef ARC_OS_WINDOWS
    #ifdef DEATH_API_ENABLE_EXPORT
        #define DEATH_API_EXPORT __declspec(dllexport)
    #else
        #define DEATH_API_EXPORT __declspec(dllimport)
    #endif
#else
    #define DEATH_API_EXPORT
#endif
//------------------------------------------------------------------------------

//----------------------------------DATA TYPES----------------------------------
typedef int8_t        DeathBool;
typedef signed char   DeathByte;
typedef unsigned char DeathUByte;
typedef int16_t       DeathInt16;
typedef uint16_t      DeathUInt16;
typedef int32_t       DeathInt32;
typedef uint32_t      DeathUInt32;
typedef int64_t       DeathInt64;
typedef uint64_t      DeathUInt64;
typedef size_t        DeathSize;
typedef float         DeathFloat;
typedef double        DeathDouble;
//------------------------------------------------------------------------------

//------------------------------------ENUMS-------------------------------------
/*!
 * \brief Defines error codes that can be returned by DeathRay API functions.
 */
enum DeathError
{
    //-----------------------------N O    E R R O R-----------------------------

    /// No error was encountered
    kDeathSuccess = 0,

    //--------------------------S O F T    E R R O R S--------------------------

    /// The operation was supplied with null handle so no operation could be
    /// performed
    kDeathErrorNullHandle = 1,
    /// There is no work to be done for the request operation
    kDeathErrorNoOperation = 2,

    //--------------------------H A R D    E R R O R S--------------------------

    /// There was insufficient machine memory to complete the operation
    kDeathErrorOutOfMemory = -1,
    /// A valid graphics state could not be acquired
    kDeathErrorInvalidGraphicsState = -2,
    /// A provided enum value was invalid for the operation
    kDeathErrorInvalidEnumValue = -3,
    /// Data tuple size is not a factor of the data size
    kDeathErrorTupleSizeNotAFactor = -4
};

enum DeathDataType
{
    kDeathByte = 0,
    kDeathUByte,
    kDeathInt16,
    kDeathUInt16,
    kDeathInt32,
    kDeathUInt32,
    kDeathInt64,
    kDeathUInt64,
    kDeathFloat,
    kDeathDouble
};

//------------------------------------------------------------------------------

#endif
