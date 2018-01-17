#include "deathray/impl/pathtracer/cpu/CPUIntersections.hpp"

#include <cmath>

#include <arcanecore/base/math/MathOperations.hpp>
#include <arcanecore/lx/AABB.hpp>
#include <arcanecore/lx/Matrix.hpp>

#include "deathray/impl/data/PackedOctree.hpp"


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
        const arc::lx::Vector3f& sphere_centre,
        DeathFloat sphere_radius,
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        DeathFloat& out_t_0,
        DeathFloat& out_t_1)
{
    // vpc is the distance from the origin of the ray to the centre of the
    // sphere
    const arc::lx::Vector3f vpc = sphere_centre - ray_origin;
    // project vpc onto the direction of the ray
    const DeathFloat tca = vpc.dot(ray_direction);
    // // is the sphere behind the ray origin?
    if(tca < 0.0F)
    {
        return false;
    }

    const DeathFloat radius2 = sphere_radius * sphere_radius;
    DeathFloat d2 = vpc.dot(vpc) - (tca * tca);
    // ray misses sphere?
    if(d2 > radius2)
    {
        return false;
    }

    // return intersection points
    const DeathFloat thc = std::sqrt(radius2 - d2);
    out_t_0 = tca - thc;
    out_t_1 = tca + thc;

    if(out_t_0 > out_t_1)
    {
        DeathFloat temp = out_t_1;
        out_t_1 = out_t_0;
        out_t_0 = temp;
    }

    if(out_t_0 < 0.0F)
    {
        out_t_0 = out_t_1;
        if(out_t_0 < 0.0F)
        {
            return false;
        }
    }

    return true;
}

bool ray_plane(
        const arc::lx::Vector3f& plane_position,
        const arc::lx::Vector3f& plane_normal,
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        DeathFloat& out_t)
{
    DeathFloat denom = (-plane_normal).dot(ray_direction);
    if(denom > 0.000001F)
    {
        const arc::lx::Vector3f p0l0 = plane_position- ray_origin;
        out_t = p0l0.dot(-plane_normal) / denom;
        return out_t >= 0.0F;
    }

    return false;
}

// TODO:: could add an optimized version of this function that takes half_size
//        and returns the intersection point
bool ray_quad(
        const arc::lx::Vector3f& plane_position,
        const arc::lx::Vector3f& plane_normal,
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        DeathFloat quad_size,
        DeathFloat& out_t)
{
    DeathFloat denom = (-plane_normal).dot(ray_direction);
    if(denom > 0.000001F)
    {
        const arc::lx::Vector3f p0l0 = plane_position - ray_origin;
        out_t = p0l0.dot(-plane_normal) / denom;
        if(out_t >= 0.0F)
        {
            arc::lx::Vector3f intersect =
                ray_origin + (ray_direction * out_t);
            arc::lx::Vector3f distance = intersect - plane_position;
            distance = distance.cwiseAbs();

            const float half_size = quad_size / 2.0F;
            if(distance(0) <= half_size &&
               distance(1) <= half_size &&
               distance(2) <= half_size)
            {
                return true;
            }
        }
    }
    return false;
}

bool ray_quad_opt(
        const arc::lx::Vector3f& plane_position,
        const arc::lx::Vector3f& plane_normal,
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        DeathFloat half_size,
        arc::lx::Vector3f& out_intersection)
{
    DeathFloat denom = (-plane_normal).dot(ray_direction);
    if(denom > 0.000001F)
    {
        const arc::lx::Vector3f p0l0 = plane_position - ray_origin;
        DeathFloat t = p0l0.dot(-plane_normal) / denom;
        if(t >= 0.0F)
        {
            out_intersection = ray_origin + (ray_direction * t);
            arc::lx::Vector3f distance = out_intersection - plane_position;
            distance = distance.cwiseAbs();

            if(distance(0) <= half_size &&
               distance(1) <= half_size &&
               distance(2) <= half_size)
            {
                return true;
            }
        }
    }
    return false;
}

bool octree_root(
        const death::PackedOctree* octree,
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        arc::lx::Vector3f& out_intersection,
        arc::lx::Vector3f& out_face_normal)
{
    const DeathFloat size = octree->get_root_size();
    const DeathFloat half_size = octree->get_root_size() / 2.0F;
    const arc::lx::Vector3f v_half_size(half_size, half_size, half_size);
    const arc::lx::Vector3f position = octree->get_position();
    const arc::lx::AABB3f bounds(
        position - v_half_size,
        position + v_half_size
    );

    // TODO: should first check if this intersects with the "actual" bounds
    //       of the spatial!

    // perform all the bounds checks up front (optimisation to avoid branching)
    // to determine which side of the octree the ray origin is on
    // note: the min check is multiplied by -2 so that if both min and max are
    //       true (1) then the result still resolves to -1 (non-zero) rather
    //       than 0
    arc::int32 x_check =
        (static_cast<arc::int32>(ray_origin(0) < bounds.min()(0)) * -2) +
        (static_cast<arc::int32>(ray_origin(0) > bounds.max()(0)));
    arc::int32 y_check =
        (static_cast<arc::int32>(ray_origin(1) < bounds.min()(1)) * -2) +
        (static_cast<arc::int32>(ray_origin(1) > bounds.max()(1)));
    arc::int32 z_check =
        (static_cast<arc::int32>(ray_origin(2) < bounds.min()(2)) * -2) +
        (static_cast<arc::int32>(ray_origin(2) > bounds.max()(2)));
    // clamp checks
    x_check = arc::math::clamp(x_check, -1, 1);
    y_check = arc::math::clamp(y_check, -1, 1);
    z_check = arc::math::clamp(z_check, -1, 1);

    // is the ray inside the bounds at all?
    if(x_check == 0 && y_check == 0 && z_check == 0)
    {
        // TODO: support internal rays
        return false;
    }

    // TODO: OPT HERE
    // TODO: can we build a jump table of some sort - or at least build the
    //       the position from this information to avoid branching
    //       TODO: can actually be avoid by always checking 3 normals
    //             where (0, 0, 0) normals fails the test

    // branch to determine which and how many of octree's root faces we have
    // intersect check
    if(z_check != 0)
    {
        if(y_check != 0)
        {
            if(x_check != 0)
            {
                arc::lx::Vector3f n1(0.0F, 0.0F, static_cast<float>(z_check));
                arc::lx::Vector3f p1 = position + (n1 * half_size);
                arc::lx::Vector3f n2(0.0F, static_cast<float>(y_check), 0.0F);
                arc::lx::Vector3f p2 = position + (n2 * half_size);
                arc::lx::Vector3f n3(static_cast<float>(x_check), 0.0F, 0.0F);
                arc::lx::Vector3f p3 = position + (n3 * half_size);
                return death::cpu::intersect::octree_faces_convex_3(
                    p1,
                    n1,
                    p2,
                    n2,
                    p3,
                    n3,
                    size,
                    ray_origin,
                    ray_direction,
                    out_intersection,
                    out_face_normal
                );
            }

            arc::lx::Vector3f n1(0.0F, 0.0F, static_cast<float>(z_check));
            arc::lx::Vector3f p1 = position + (n1 * half_size);
            arc::lx::Vector3f n2(0.0F, static_cast<float>(y_check), 0.0F);
            arc::lx::Vector3f p2 = position + (n2 * half_size);
            return death::cpu::intersect::octree_faces_convex_2(
                p1,
                n1,
                p2,
                n2,
                size,
                ray_origin,
                ray_direction,
                out_intersection,
                out_face_normal
            );
        }
        if(x_check != 0)
        {
            arc::lx::Vector3f n1(0.0F, 0.0F, static_cast<float>(z_check));
            arc::lx::Vector3f p1 = position + (n1 * half_size);
            arc::lx::Vector3f n2(static_cast<float>(x_check), 0.0F, 0.0F);
            arc::lx::Vector3f p2 = position + (n2 * half_size);
            return death::cpu::intersect::octree_faces_convex_2(
                p1,
                n1,
                p2,
                n2,
                size,
                ray_origin,
                ray_direction,
                out_intersection,
                out_face_normal
            );
        }
        arc::lx::Vector3f n1(0.0F, 0.0F, static_cast<float>(z_check));
        arc::lx::Vector3f p1 = position + (n1 * half_size);
        return death::cpu::intersect::octree_faces_convex_1(
            p1,
            n1,
            size,
            ray_origin,
            ray_direction,
            out_intersection,
            out_face_normal
        );
    }
    if(y_check != 0)
    {
        if(x_check != 0)
        {
            arc::lx::Vector3f n1(0.0F, static_cast<float>(y_check), 0.0F);
            arc::lx::Vector3f p1 = position + (n1 * half_size);
            arc::lx::Vector3f n2(static_cast<float>(x_check), 0.0F, 0.0F);
            arc::lx::Vector3f p2 = position + (n2 * half_size);
            return death::cpu::intersect::octree_faces_convex_2(
                p1,
                n1,
                p2,
                n2,
                size,
                ray_origin,
                ray_direction,
                out_intersection,
                out_face_normal
            );
        }
        arc::lx::Vector3f n1(0.0F, static_cast<float>(y_check), 0.0F);
        arc::lx::Vector3f p1 = position + (n1 * half_size);
        return death::cpu::intersect::octree_faces_convex_1(
            p1,
            n1,
            size,
            ray_origin,
            ray_direction,
            out_intersection,
            out_face_normal
        );
    }
    if(x_check != 0)
    {
        arc::lx::Vector3f n1(static_cast<float>(x_check), 0.0F, 0.0F);
        arc::lx::Vector3f p1 = position + (n1 * half_size);
        return death::cpu::intersect::octree_faces_convex_1(
            p1,
            n1,
            size,
            ray_origin,
            ray_direction,
            out_intersection,
            out_face_normal
        );
    }

    return false;
}

bool octree_faces_convex_1(
        const arc::lx::Vector3f& position,
        const arc::lx::Vector3f& normal,
        DeathFloat size,
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        arc::lx::Vector3f& out_intersection,
        arc::lx::Vector3f& out_face_normal)
{
    const DeathFloat half_size = size / 2.0F;

    // TODO: ray quad intersection needs fixing?

    // TODO: OPT HERE: could this intersection test be turned into a single
    //                 function to save computation?
    if(death::cpu::intersect::ray_quad_opt(
        position,
        normal,
        ray_origin,
        ray_direction,
        half_size,
        out_intersection
    ))
    {
        out_face_normal = normal;
        return true;
    }

    return false;
}

bool octree_faces_convex_2(
        const arc::lx::Vector3f& position_1,
        const arc::lx::Vector3f& normal_1,
        const arc::lx::Vector3f& position_2,
        const arc::lx::Vector3f& normal_2,
        DeathFloat size,
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        arc::lx::Vector3f& out_intersection,
        arc::lx::Vector3f& out_face_normal)
{
    const DeathFloat half_size = size / 2.0F;

    // TODO: ray quad intersection needs fixing?

    // TODO: OPT HERE: could this intersection test be turned into a single
    //                 function to save computation?
    if(death::cpu::intersect::ray_quad_opt(
        position_1,
        normal_1,
        ray_origin,
        ray_direction,
        half_size,
        out_intersection
    ))
    {
        out_face_normal = normal_1;
        return true;
    }

    if(death::cpu::intersect::ray_quad_opt(
        position_2,
        normal_2,
        ray_origin,
        ray_direction,
        half_size,
        out_intersection
    ))
    {
        out_face_normal = normal_2;
        return true;
    }

    return false;
}

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
        arc::lx::Vector3f& out_face_normal)
{
    const DeathFloat half_size = size / 2.0F;

    // TODO: ray quad intersection needs fixing?

    // TODO: OPT HERE: could this intersection test be turned into a single
    //                 function to save computation?
    if(death::cpu::intersect::ray_quad_opt(
        position_1,
        normal_1,
        ray_origin,
        ray_direction,
        half_size,
        out_intersection
    ))
    {
        out_face_normal = normal_1;
        return true;
    }

    if(death::cpu::intersect::ray_quad_opt(
        position_2,
        normal_2,
        ray_origin,
        ray_direction,
        half_size,
        out_intersection
    ))
    {
        out_face_normal = normal_2;
        return true;
    }

    if(death::cpu::intersect::ray_quad_opt(
        position_3,
        normal_3,
        ray_origin,
        ray_direction,
        half_size,
        out_intersection
    ))
    {
        out_face_normal = normal_3;
        return true;
    }


    return false;
}

void octree_faces_concave(
        const arc::lx::Vector3f& position,
        DeathFloat half_size,
        const arc::lx::Vector3f& face_normal,
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        arc::lx::Vector3f& out_intersection)
{
    const DeathFloat size = half_size * 2.0F;

    // get a inverted copy of the normal since the given normal points outside
    // of the cube
    arc::lx::Vector3f n = -face_normal;

    // TODO: can we get away without the change of basis? (without branching)
    // change the basis of the ray origin and direction to match to match the
    // co-ordinate space of the normal
    arc::lx::Matrix33f change_of_basis;
    change_of_basis <<
        n(0), n(1), n(2),
        n(1), n(2), n(0),
        n(2), n(0), n(1);
    arc::lx::Matrix33f invert_change_of_basis = change_of_basis.inverse();

    arc::lx::Vector3f cell_r_origin = change_of_basis * (ray_origin - position);
    const arc::lx::Vector3f cell_r_direction = change_of_basis * ray_direction;

    const DeathFloat x_multiplier =
        size / std::abs(cell_r_direction(0));
    const arc::lx::Vector3f cell_x_exit =
            cell_r_origin + (cell_r_direction * x_multiplier);

    const DeathFloat dx = std::abs(cell_x_exit(0));
    const DeathFloat dy = std::abs(cell_x_exit(1));
    const DeathFloat dz = std::abs(cell_x_exit(2));

    if(dx >= dy && dx >= dz)
    {
        out_intersection = position + (invert_change_of_basis * cell_x_exit);
        return;
    }

    DeathFloat y_dis = cell_r_origin(1) + half_size;
    // TODO: pretty sure can remove this if (can be done with bitwise
    //       at worse case (using std::ceil(cell_r_direction(0)) operators)
    if(cell_r_direction(1) > 0.0F)
    {
        y_dis = size - y_dis;
    }
    const DeathFloat y_multiplier = (y_dis) / std::abs(cell_r_direction(1));

    DeathFloat z_dis = cell_r_origin(2) + half_size;
    // TODO: pretty sure can remove this if (can be done with bitwise
    //       at worse case (using std::ceil(cell_r_direction(0)) operators)
    if(cell_r_direction(2) > 0.0F)
    {
        z_dis = size - z_dis;
    }
    const DeathFloat z_multiplier = (z_dis) / std::abs(cell_r_direction(2));

    if(y_multiplier <= z_multiplier)
    {
        const arc::lx::Vector3f cell_y_exit =
            cell_r_origin + (cell_r_direction * y_multiplier);
        out_intersection = position + (invert_change_of_basis * cell_y_exit);
        return;
    }

    const arc::lx::Vector3f cell_z_exit =
        cell_r_origin + (cell_r_direction * z_multiplier);
    out_intersection = position + (invert_change_of_basis * cell_z_exit);
}

} // namespace intersect
} // namespace cpu
} // namespace death
