#include "pxtrace/Sphere.hpp"

#include <arcanecore/gm/VectorMath.hpp>


//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

Sphere::Sphere(const arc::gm::SimdVector3f& position, float radius)
    : m_position(position)
    , m_radius  (radius)
    , m_radius2 (radius * radius)
{
}

Sphere::Sphere(const Sphere& other)
    : m_position(other.m_position)
    , m_radius  (other.m_radius)
    , m_radius2 (other.m_radius2)
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

Sphere::~Sphere()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

bool Sphere::intersects(
        const arc::gm::SimdVector3f& ray_origin,
        const arc::gm::SimdVector3f& ray_direction,
        float& t0,
        float& t1) const
{
    arc::gm::SimdVector3f l = m_position - ray_origin;

    float tca = arc::gm::dot(l, ray_direction);
    if(tca < 0.0F)
    {
        return false;
    }

    float d_2 = arc::gm::dot(l, l) - (tca * tca);
    if(d_2 > m_radius2)
    {
        return false;
    }

    float thc = std::sqrt(m_radius2 - d_2);
    t0 = tca - thc;
    t1 = tca + thc;
    return true;
}
