/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 *
 * \brief Relating to geometric data for the DeathRay renderer.
 */
#ifndef DEATHRAY_API_GEOMETRIC_H_
#define DEATHRAY_API_GEOMETRIC_H_

#include "deathray/api/API.h"
#include "deathray/api/VBO.h"


DEATH_API_NO_MANGLE_BEGIN;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                                     HANDLE
//------------------------------------------------------------------------------

/*!
 * \brief The type that is used to provide a handle to DeathRay geometric data.
 */
typedef struct DeathGeometric* DeathGeometricHandle;

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

// TODO: DOC
DEATH_API_EXPORT DeathError death_geo_gen(
        DeathSize count,
        DeathGeometricHandle* geometrics);

// TODO: DOC
DEATH_API_EXPORT DeathError death_geo_delete(
        DeathSize count,
        DeathGeometricHandle* geometrics);

// TODO: DOC
DEATH_API_EXPORT DeathError death_geo_attach_vbo(
        DeathGeometricHandle geometric,
        DeathUInt32 index,
        DeathVBOHandle vbo);

// TODO: DOC
DEATH_API_EXPORT DeathError death_geo_remove_vbo(
        DeathGeometricHandle geometric,
        DeathVBOHandle vbo);

// TODO: get vbos

//------------------------------------------------------------------------------
DEATH_API_NO_MANGLE_END;

#endif
