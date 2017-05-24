/*!
 * \file
 * \author David Saxon
 */
#ifndef PXTRACE_SPHERE_HPP_
#define PXTRACE_SPHERE_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/base/memory/Alignment.hpp>
#include <arcanecore/gm/Vector.hpp>


class Sphere
    : private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
    , public arc::memory::AlignedBase<Sphere>
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    Sphere(const arc::gm::SimdVector3f& position, float radius);

    Sphere(const Sphere& other);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    ~Sphere();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    bool intersects(
            const arc::gm::SimdVector3f& ray_origin,
            const arc::gm::SimdVector3f& ray_direction,
            float& t0,
            float& t1) const;

private:

    //--------------------------------------------------------------------------
    //                             PRIVATE ATTRIBUTES
    //--------------------------------------------------------------------------

    arc::gm::SimdVector3f m_position;
    float m_radius;
    float m_radius2;
};

#endif
