/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 *
 * \brief Relating to vertex buffer objects for the DeathRay renderer.
 */
#ifndef DEATHRAY_API_VBO_H_
#define DEATHRAY_API_VBO_H_

#include "deathray/api/API.h"


DEATH_API_NO_MANGLE_BEGIN;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                                     HANDLE
//------------------------------------------------------------------------------

/*!
 * \brief The type that is used to provide a handle to a DeathRay vertex buffer
 *        object.
 */
typedef struct DeathVBO* DeathVBOHandle;

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

// TODO: DOC
DEATH_API_EXPORT DeathError death_vbo_gen(
        DeathSize count,
        DeathVBOHandle* vbos);

// TODO: DOC
DEATH_API_EXPORT DeathError death_vbo_delete(
        DeathSize count,
        DeathVBOHandle* vbos);

// TODO: support offsets and strides here
// TODO: support non-ownership
// TODO: could take static flag
// TODO: DOC
DEATH_API_EXPORT DeathError death_vbo_set_data(
        DeathVBOHandle vbo,
        DeathDataType data_type,
        DeathSize size,
        DeathSize tuple_size,
        const void* data);

//------------------------------------------------------------------------------
DEATH_API_NO_MANGLE_END;

#endif
