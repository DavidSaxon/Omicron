#include "deathray/impl/pathtracer/cpu/CPUIntersections.hpp"

#include <cmath>


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
        float& t1)
{
    // vpc is the distance from the origin of the ray to the centre of the
    // sphere
    const arc::lx::Vector3f vpc = sphere_centre - ray_origin;
    // project vpc onto the direction of the ray
    const float tca = vpc.dot(ray_direction);
    // // is the sphere behind the ray origin?
    if(tca < 0.0F)
    {
        return false;
    }

    const float radius2 = sphere_radius * sphere_radius;
    float d2 = vpc.dot(vpc) - (tca * tca);
    d2 = fabs(d2);
    // ray misses sphere?
    if(d2 > radius2)
    {
        return false;
    }

    // return intersection points
    const float thc = std::sqrt(radius2 - d2);
    t0 = tca - thc;
    t1 = tca + thc;

    if(t0 > t1)
    {
        float temp = t1;
        t1 = t0;
        t0 = temp;
    }

    if(t0 < 0.0F)
    {
        t0 = t1;
        if(t0 < 0.0F)
        {
            return false;
        }
    }

    return true;

    //--------------------------------------------------------------------------

    // const arc::lx::Vector3f m = ray_origin - sphere_centre;
    // float b = m.dot(ray_direction);
    // float c = m.dot(m) - (b * b);

    // if(c > 0.0F && b > 0.0F)
    // {
    //     return false;
    // }

    // float discr = (sphere_radius * sphere_radius) - c;
    // if(discr < 0.0F)
    // {
    //     return false;
    // }

    // t0 = -b - std::sqrt(discr);

    // // TODO: ?

    // return true;
}

bool ray_plane(
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        const arc::lx::Vector3f& plane_position,
        const arc::lx::Vector3f& plane_normal,
        float& t)
{
    float denom = (-plane_normal).dot(ray_direction);
    if(denom > 0.000001F)
    {
        const arc::lx::Vector3f p0l0 = plane_position- ray_origin;
        t = p0l0.dot(-plane_normal) / denom;
        return t >= 0.0F;
    }

    return false;
}

// TODO: this function should return the intersection point
bool ray_quad(
        const arc::lx::Vector3f& ray_origin,
        const arc::lx::Vector3f& ray_direction,
        const arc::lx::Vector3f& plane_position,
        const arc::lx::Vector3f& plane_normal,
        float quad_size,
        float& t)
{
    float denom = (-plane_normal).dot(ray_direction);
    if(denom > 0.000001F)
    {
        const arc::lx::Vector3f p0l0 = plane_position- ray_origin;
        t = p0l0.dot(-plane_normal) / denom;
        if(t >= 0.0F)
        {
            static const arc::lx::Vector3f one(1.0F, 1.0F, 1.0F);

            arc::lx::Vector3f intersect =
                ray_origin + (ray_direction * t);
            // TODO: needs fixing to handle x and y size
            arc::lx::Vector3f check_intersect = plane_normal.cwiseAbs();
            check_intersect = one - check_intersect;
            check_intersect(0) = intersect(0) * check_intersect(0);
            check_intersect(1) = intersect(1) * check_intersect(1);
            check_intersect(2) = intersect(2) * check_intersect(2);
            check_intersect = check_intersect.cwiseAbs();

            float half_size = quad_size / 2.0F;
            if(check_intersect(0) <= half_size &&
               check_intersect(1) <= half_size &&
               check_intersect(2) <= half_size)
            {
                return true;
            }
        }
    }

    return false;
}

} // namespace intersect
} // namespace cpu
} // namespace death
