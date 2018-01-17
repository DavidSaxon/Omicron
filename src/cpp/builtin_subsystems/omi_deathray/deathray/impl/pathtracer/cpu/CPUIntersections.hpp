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

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class PackedOctree;

namespace cpu
{
namespace intersect
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

// TODO: DOC
bool ray_sphere_geometric(
        const arc::lx::Vector3f& sphere_centre,
        DeathFloat sphere_radius,
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        DeathFloat& out_t_0,
        DeathFloat& out_t_1);

// TODO: DOC
bool ray_plane(
        const arc::lx::Vector3f& plane_position,
        const arc::lx::Vector3f& plane_normal,
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        DeathFloat& out_t);

// TODO: DOC
bool ray_quad(
        const arc::lx::Vector3f& plane_position,
        const arc::lx::Vector3f& plane_normal,
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        DeathFloat quad_size,
        DeathFloat& out_t);

// TODO: DOC
bool ray_quad_opt(
        const arc::lx::Vector3f& plane_position,
        const arc::lx::Vector3f& plane_normal,
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        DeathFloat half_size,
        arc::lx::Vector3f& out_intersection);

// TODO: DOC
// TODO: face normal is return in axis aligned space?
bool octree_root(
        const death::PackedOctree* octree,
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        arc::lx::Vector3f& out_intersection,
        arc::lx::Vector3f& out_face_normal);

// TODO: DOC
bool octree_faces_convex_1(
        const arc::lx::Vector3f& position,
        const arc::lx::Vector3f& normal,
        DeathFloat size,
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        arc::lx::Vector3f& out_intersection,
        arc::lx::Vector3f& out_face_normal);

// TODO: DOC
bool octree_faces_convex_2(
        const arc::lx::Vector3f& position_1,
        const arc::lx::Vector3f& normal_1,
        const arc::lx::Vector3f& position_2,
        const arc::lx::Vector3f& normal_2,
        DeathFloat size,
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        arc::lx::Vector3f& out_intersection,
        arc::lx::Vector3f& out_face_normal);

// TODO: DOC
bool octree_faces_convex_3(
        const arc::lx::Vector3f& position_1,
        const arc::lx::Vector3f& normal_1,
        const arc::lx::Vector3f& position_2,
        const arc::lx::Vector3f& normal_2,
        const arc::lx::Vector3f& position_3,
        const arc::lx::Vector3f& normal_3,
        DeathFloat size,
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        arc::lx::Vector3f& out_intersection,
        arc::lx::Vector3f& out_face_normal);

// TODO: DOC
void octree_faces_concave(
        const arc::lx::Vector3f& position,
        DeathFloat half_size,
        const arc::lx::Vector3f& face_normal,
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        arc::lx::Vector3f& out_intersection);

} // namespace intersect
} // namespace cpu
} // namespace death

#endif
