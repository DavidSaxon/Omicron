/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 *
 * \brief Relating to a DeathRay scene.
 */
#ifndef DEATHRAY_API_SCENE_H_
#define DEATHRAY_API_SCENE_H_

#include "deathray/api/API.h"
#include "deathray/api/Camera.h"
#include "deathray/api/Spatial.h"


DEATH_API_NO_MANGLE_BEGIN;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                                     HANDLE
//------------------------------------------------------------------------------

/*!
 * \brief The type that is used to provide a handle to DeathRay spatial
 *        entities.
 */
typedef struct DeathScene* DeathSceneHandle;

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

// TODO: DOC
DEATH_API_EXPORT DeathError death_scene_gen(DeathSceneHandle* scene);

// TODO: DOC
DEATH_API_EXPORT DeathError death_scene_delete(DeathSceneHandle* scene);

// TODO: DOC
DEATH_API_EXPORT DeathError death_scene_set_resolution(
        DeathSceneHandle scene,
        DeathUInt32 width,
        DeathUInt32 height);

// TODO: DOC
DEATH_API_EXPORT DeathError death_scene_set_camera(
        DeathSceneHandle scene,
        DeathCameraHandle camera);

// TODO: REMOVE FROM API
DEATH_API_EXPORT DeathError death_scene_set_debug_camera(
        DeathSceneHandle scene,
        DeathCameraHandle camera);

// TODO: DOC
DEATH_API_EXPORT DeathError death_scene_add_spatial(
        DeathSceneHandle scene,
        DeathSpatialHandle spatial);

// TODO: DOC
DEATH_API_EXPORT DeathError death_scene_remove_spatial(
        DeathSceneHandle scene,
        DeathSpatialHandle spatial);

// TODO: DOC
DEATH_API_EXPORT DeathError death_scene_render(DeathSceneHandle scene);

// TODO: settings

//------------------------------------------------------------------------------
DEATH_API_NO_MANGLE_END;

#endif
