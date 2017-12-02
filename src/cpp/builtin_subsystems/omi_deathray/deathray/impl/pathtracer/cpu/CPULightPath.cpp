/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/pathtracer/cpu/CPULightPath.hpp"

#include <arcanecore/lx/Alignment.hpp>
#include <arcanecore/lx/MatrixMath44f.hpp>

#include "deathray/impl/Camera.hpp"
#include "deathray/impl/Octree.hpp"
#include "deathray/impl/Scene.hpp"
#include "deathray/impl/pathtracer/cpu/CPUIntersections.hpp"
#include "deathray/impl/pathtracer/cpu/CPURayHierarchy.hpp"

// TODO: REMOVE ME
#include <iostream>


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
        // if(debug_camera)
        // {
            death::Camera* dc = scene->get_debug_camera();
            camera_transform = dc->get_transform();
        // }

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
        out_colour = octree_trace(scene, ray_origin, ray_direction, out_rays);

        // TODO:
        return out_colour;
    }

    arc::lx::Vector3f octree_trace(
            death::Scene* scene,
            const arc::lx::Vector3f& ray_origin,
            const arc::lx::Vector3f& ray_direction,
            RayHierarchy* out_rays)
    {
        for(death::Spatial* spatial : scene->get_spatials())
        {
            // get the octree
            death::Octree* octree = scene->get_or_create_octree(spatial);

            // TODO:
            const arc::lx::Vector3f octree_position(
                0.0F,
                0.0F,
                0.0F
                // octree->get_root_size() * 0.5F
            );

            // TODO: testing
            float t0 = 0.0F;
            float t1 = 0.0F;
            // bool c = false;
            bool c = death::cpu::intersect::ray_sphere_geometric(
                ray_origin,
                ray_direction,
                octree_position,
                octree->get_root_size() * 0.5,
                // 1.0F,
                t0,
                t1
            );
            // bool c = death::cpu::intersect::ray_quad(
            //     ray_origin,
            //     ray_direction,
            //     octree_position,
            //     arc::lx::Vector3f(0.0F, 0.0F, 1.0F),
            //     octree->get_root_size(),
            //     t0
            // );

            if(c)
            {
                if(out_rays != nullptr)
                {
                    out_rays->end = ray_origin + (ray_direction * t0);
                }
                return arc::lx::Vector3f(1.0F, 0.0F, 0.0F);
            }
        };

        // no collision
        if(out_rays != nullptr)
        {
            // TODO: should be infinity?
            out_rays->end = ray_origin + (ray_direction * 1000.0F);
        }
        return arc::lx::Vector3f(0.0F, 0.0F, 0.0F);
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
