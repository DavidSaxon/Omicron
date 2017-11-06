/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 *
 * \brief Relating to spatial entities within the DeathRay renderer.
 */
#ifndef DEATHRAY_API_SPATIAL_H_
#define DEATHRAY_API_SPATIAL_H_

#include "deathray/api/API.h"
#include "deathray/api/Geometric.h"


DEATH_API_NO_MANGLE_BEGIN;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                                     HANDLE
//------------------------------------------------------------------------------

/*!
 * \brief The type that is used to provide a handle to DeathRay spatial
 *        entities.
 */
typedef struct DeathSpatial* DeathSpatialHandle;

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

// TODO: DOC
DEATH_API_EXPORT DeathError death_spatial_gen(
        DeathSize count,
        DeathSpatialHandle* spatials);

// TODO: DOC
DEATH_API_EXPORT DeathError death_spatial_delete(
        DeathSize count,
        DeathSpatialHandle* spatials);

// TODO: DOC
DEATH_API_EXPORT DeathError death_spatial_attach_geo(
        DeathSpatialHandle spatial,
        DeathGeometricHandle geometric);

// TODO: DOC
DEATH_API_EXPORT DeathError death_spatial_remove_geo(
        DeathSpatialHandle spatial,
        DeathGeometricHandle geometric);

//------------------------------------------------------------------------------
DEATH_API_NO_MANGLE_END;

#endif
