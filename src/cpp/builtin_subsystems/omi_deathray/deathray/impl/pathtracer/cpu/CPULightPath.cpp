/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/pathtracer/cpu/CPULightPath.hpp"

#include <limits>

#include <arcanecore/lx/Alignment.hpp>
#include <arcanecore/lx/MatrixMath44f.hpp>

#include "deathray/impl/Camera.hpp"
#include "deathray/impl/Globals.hpp"
#include "deathray/impl/Octree.hpp"
#include "deathray/impl/Scene.hpp"
#include "deathray/impl/data/PackedOctree.hpp"
#include "deathray/impl/pathtracer/cpu/CPUIntersections.hpp"
#include "deathray/impl/pathtracer/cpu/CPURayHierarchy.hpp"


namespace death
{
namespace cpu
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class LightPath::LightPathImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the focal length of the camera this light path was generated for
    float m_focal_length;
    // the position on the image plane this path crosses through
    arc::lx::Vector2f m_plane_position;

public:

    ARC_LX_ALIGNED_NEW;

    //--------------------------C O N S T R U C T O R---------------------------

    LightPathImpl(float focal_length, const arc::lx::Vector2f& plane_position)
        : m_focal_length  (focal_length)
        , m_plane_position(plane_position)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~LightPathImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    arc::lx::Vector3f trace(
            death::Scene* scene,
            RayHierarchy* out_rays,
            bool debug_camera)
    {
        // determine the initial ray position and direction
        arc::lx::Vector3f ray_origin(
            m_plane_position(0) * 0.1F,
            m_plane_position(1) * 0.1F,
            -m_focal_length * 0.1F
        );
        arc::lx::Vector3f ray_direction(
            m_plane_position(0),
            m_plane_position(1),
            -m_focal_length
        );
        ray_direction.normalize();

        death::Camera* camera = scene->get_camera();
        arc::lx::Matrix44f camera_transform = camera->get_transform();

        // TODO: sort this out
        // apply the camera position?
        if(debug_camera)
        {
            death::Camera* dc = scene->get_debug_camera();
            camera_transform = dc->get_transform();
        }

        arc::lx::Vector4f origin4(0.0F, 0.0F, 0.0F, 1.0F);
        origin4.block<3, 1>(0, 0) = ray_origin;
        origin4 = camera_transform * origin4;
        ray_origin = origin4.block<3, 1>(0, 0);

        arc::lx::Vector4f direction4(0.0F, 0.0F, 0.0F, 0.0F);
        direction4.block<3, 1>(0, 0) = ray_direction;
        direction4 = camera_transform * direction4;
        ray_direction = direction4.block<3, 1>(0, 0);

        if(out_rays != nullptr)
        {
            out_rays->start = ray_origin;
        }

        // the output colour
        arc::lx::Vector3f out_colour(0.0F, 0.0F, 0.0F);

        // trace into octrees
        out_colour = scene_trace(scene, ray_origin, ray_direction, out_rays);

        // TODO:
        return out_colour;
    }

    arc::lx::Vector3f scene_trace(
            death::Scene* scene,
            const arc::lx::Vector3f& ray_origin_,
            const arc::lx::Vector3f& ray_direction_,
            RayHierarchy* out_rays)
    {
        // make new mutable copies of the ray parameters
        arc::lx::Vector3f ray_origin = ray_origin_;
        arc::lx::Vector3f ray_direction = ray_direction_;
        // the colour after tracing
        arc::lx::Vector3f colour(0.0F, 0.0F, 0.0F);

        for(death::Spatial* spatial : scene->get_spatials())
        {
            // TODO: right now we're just using the first octree we encounter
            //       but we need to an initial pass of all octrees (or at least
            //       all octrees in a divided space to figure out which we one
            //       we should be tracing)

            // get the octree
            death::Octree* octree = scene->get_or_create_octree(spatial);
            // get the packed data for the octree
            const death::PackedOctree* packed = octree->get_packed_data();

            // trace
            if(octree_trace(
                    packed,
                    ray_origin,
                    ray_direction,
                    colour,
                    out_rays))
            {
                return colour;
            }


        //     // TODO:
        //     const arc::lx::Vector3f octree_position(
        //         0.0F,
        //         0.0F,
        //         // 0.0F
        //         octree->get_root_size() * 0.5F
        //     );

        //     // TODO: testing
        //     float t0 = 0.0F;
        //     float t1 = 0.0F;
        //     bool c = death::cpu::intersect::ray_quad(
        //         ray_origin,
        //         ray_direction,
        //         octree_position,
        //         arc::lx::Vector3f(0.0F, 0.0F, 1.0F),
        //         octree->get_root_size(),
        //         t0
        //     );

        //     if(c)
        //     {
        //         if(out_rays != nullptr)
        //         {
        //             out_rays->end = ray_origin + (ray_direction * t0);
        //         }
        //         return arc::lx::Vector3f(1.0F, 0.0F, 0.0F);
        //     }
        };

        // no collision
        if(out_rays != nullptr)
        {
            // TODO: should be infinity?
            out_rays->end = ray_origin + (ray_direction * 1000.0F);
        }
        return arc::lx::Vector3f(0.0F, 0.0F, 0.0F);
    }

    bool octree_trace(
            const death::PackedOctree* octree,
            arc::lx::Vector3f& ray_origin,
            arc::lx::Vector3f& ray_direction,
            arc::lx::Vector3f& colour,
            RayHierarchy* out_rays)
    {
        // TODO: we should actually jump to the first collision within
        //       the actual bounds

        arc::lx::Vector3f intersection;
        arc::lx::Vector3f normal;
        // determine if there's a intersection with the root level of the octree
        if(death::cpu::intersect::octree_root(
            octree,
            ray_origin,
            ray_direction,
            intersection,
            normal
        ))
        {
            // TODO: REMOVE ME
            colour = intersection.cwiseAbs();
            if(out_rays != nullptr)
            {
                 out_rays->end = intersection;
            }

            return march_octree(
                octree,
                normal,
                intersection,
                ray_origin,
                ray_direction,
                colour,
                out_rays
            );
        }

        return false;
    }

    // this function must be non-recursive so it can be replicated on the GPU
    bool march_octree(
            const death::PackedOctree* octree,
            const arc::lx::Vector3f& face_normal,
            const arc::lx::Vector3f& initial_intersection,
            arc::lx::Vector3f& ray_origin,
            arc::lx::Vector3f& ray_direction,
            arc::lx::Vector3f& colour,
            RayHierarchy* out_rays)
    {
        const float size = octree->get_root_size();
        const float half_size = size / 2.0F;
        const arc::lx::Vector3f half_size_v(half_size, half_size, half_size);

        // treat the -x -y -z (????) corner as 0, 0, 0
        arc::lx::Vector3f np_intersect = initial_intersection + half_size_v;
        // remove transformation
        np_intersect -= octree->get_position();
        // move into normalized space
        np_intersect /= size;

        // TODO: REMOVE ME
        colour = np_intersect;

        // TODO: we need to be very careful about these intersections -
        //       we should really perform all checks from the center of the
        //       minimum sized cell rather than the edges

        // get the address of the octant this is intersecting with
        bool x = np_intersect(0) > 0.5F;
        bool y = np_intersect(1) > 0.5F;
        bool z = np_intersect(2) > 0.5F;
        // TODO: Packed octree could handle this
        DeathSize octant_address = 12 + x + (y * 2) + (z* 4);

        // TODO: trace into multi level octants

        // TODO: CLEAN UP THIS LOGIC
        DeathFloat empty = octree->get_value(octant_address);
        if(empty <= std::numeric_limits<DeathFloat>::epsilon())
        {
            const DeathFloat quater_size = half_size / 2.0F;
            // calculate the position of the center of the octant
            arc::lx::Vector3f octant_position = octree->get_position();
            // TODO:
            // octant_position(0) += -quater_size + (x * half_size);
            // octant_position(1) += -quater_size + (y * half_size);
            // octant_position(2) += -quater_size + (z * half_size);
            if(x)
            {
                octant_position(0) += quater_size;
            }
            else
            {
                octant_position(0) -= quater_size;
            }
            if(y)
            {
                octant_position(1) += quater_size;
            }
            else
            {
                octant_position(1) -= quater_size;
            }
            if(z)
            {
                octant_position(2) += quater_size;
            }
            else
            {
                octant_position(2) -= quater_size;
            }

            // TODO: should all of these computations be done in normalised
            //       octree space
            // compute the next cell intersection
            arc::lx::Vector3f next_intersect;
            // TODO: somehow recurse
            death::cpu::intersect::octree_faces_concave(
                octant_position,
                quater_size,
                face_normal,
                initial_intersection,
                ray_direction,
                next_intersect
            );

            // TODO: check for ray exiting

            // TODO: determine the next octant to trace

            colour = next_intersect.cwiseAbs();
            if(out_rays != nullptr)
            {
                out_rays->end = next_intersect;
            }
            return true;
        }

        // TODO:
        return true;
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

LightPath::LightPath(
        float focal_length,
        const arc::lx::Vector2f& plane_position)
    : m_impl(new LightPathImpl(focal_length, plane_position))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

LightPath::~LightPath()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

arc::lx::Vector3f LightPath::trace(
        death::Scene* scene,
        RayHierarchy* out_rays,
        bool debug_camera)
{
    return m_impl->trace(scene, out_rays, debug_camera);
}

} // namespace cpu
} // namespace death
