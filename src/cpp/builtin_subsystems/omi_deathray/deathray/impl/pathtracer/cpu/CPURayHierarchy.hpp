/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_IMPL_PATHTRACER_CPU_RAYHIERARCHY_HPP_
#define DEATHRAY_IMPL_PATHTRACER_CPU_RAYHIERARCHY_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/lx/Alignment.hpp>
#include <arcanecore/lx/Vector.hpp>

#include "deathray/api/API.h"


namespace death
{
namespace cpu
{

/*!
 * \brief Object that describes the hierarchy of rays that were traced in order
 *        to find a path from light sources to a point on the image plane.
 */
class RayHierarchy
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    ARC_LX_ALIGNED_NEW;

    //--------------------------------------------------------------------------
    //                             PUBLIC ATTRIBUTES
    //--------------------------------------------------------------------------

    // the starting point of the ray (in world space)
    arc::lx::Vector3f start;
    // the ending point of the ray (in world space)
    arc::lx::Vector3f end;

    // the child rays of this ray
    std::vector<RayHierarchy*> children;

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new empty RayHierarchy.
     */
    RayHierarchy();

    /*!
     * \brief Creates a new hierarchical ray with the given starting and ending
     *        points.
     */
    RayHierarchy(const arc::lx::Vector3f& start_, arc::lx::Vector3f& end_);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~RayHierarchy();
};

} // namespace cpu
} // namespace death

#endif
