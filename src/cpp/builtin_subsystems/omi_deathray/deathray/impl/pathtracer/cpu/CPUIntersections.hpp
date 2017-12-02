/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_IMPL_PATHTRACER_CPU_INTERSECTIONS_HPP_
#define DEATHRAY_IMPL_PATHTRACER_CPU_INTERSECTIONS_HPP_

#include <arcanecore/lx/Vector.hpp>

#include "deathray/api/API.h"


namespace death
{
namespace cpu
{
namespace intersect
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

bool ray_sphere_geometric(
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        const arc::lx::Vector3f& sphere_centre,
        float sphere_radius,
        float& t0,
        float& t1);

bool ray_plane(
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        const arc::lx::Vector3f& plane_position,
        const arc::lx::Vector3f& plane_normal,
        float& t);

bool ray_quad(
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        const arc::lx::Vector3f& plane_position,
        const arc::lx::Vector3f& plane_normal,
        float quad_size,
        float& t);

} // namespace intersect
} // namespace cpu
} // namespace death

#endif
