/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 *
 * \brief Relating to cameras for the DeathRay renderer.
 */
#ifndef DEATHRAY_API_CAMERA_H_
#define DEATHRAY_API_CAMERA_H_

#include "deathray/api/API.h"


DEATH_API_NO_MANGLE_BEGIN;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                                     HANDLE
//------------------------------------------------------------------------------

/*!
 * \brief The type that is used to provide a handle to DeathRay camera objects.
 */
typedef struct DeathCamera* DeathCameraHandle;

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

// TODO: DOC
DEATH_API_EXPORT DeathError death_cam_gen(
        DeathSize count,
        DeathCameraHandle* cameras);

// TODO: DOC
DEATH_API_EXPORT DeathError death_cam_delete(
        DeathSize count,
        DeathCameraHandle* cameras);

// TODO: DPC
DEATH_API_EXPORT DeathError death_cam_set_transform(
        DeathCameraHandle camera,
        DeathFloat* matrix44);

// TODO: DOC
DEATH_API_EXPORT DeathError death_cam_set_properties(
        DeathCameraHandle camera,
        DeathFloat focal_length,
        DeathFloat filmback_origin_x,
        DeathFloat filmback_origin_y,
        DeathFloat filmback_width,
        DeathFloat filmback_height);

//------------------------------------------------------------------------------
DEATH_API_NO_MANGLE_END;

#endif
