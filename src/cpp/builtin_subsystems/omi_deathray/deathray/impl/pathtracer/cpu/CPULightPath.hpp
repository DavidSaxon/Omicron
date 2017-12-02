/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_IMPL_PATHTRACER_CPU_LIGHTPATH_HPP_
#define DEATHRAY_IMPL_PATHTRACER_CPU_LIGHTPATH_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/lx/Vector.hpp>

#include "deathray/api/API.h"

namespace death
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class Scene;

namespace cpu
{

class RayHierarchy;


/*!
 * \brief Object used to trace paths of light from the scene to a point on the
 *        image plane (i.e. camera sensor).
 */
class LightPath
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new Light path for the given focal length and position
     *        on the image plane (relative to the origin).
     */
    LightPath(float focal_length, const arc::lx::Vector2f& plane_position);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~LightPath();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Traces this path for the given scene.
     *
     * \param scene The scene to path trace.
     * \param out_rays A data structure which will return the info of the rays
     *                 that were traced for this path. If null this data will
     *                 not be recored.
     * \param debug_camera If this path should be traced from the debug camera.
     *
     * \return The final colour on the image plane of this ray.
     */
    arc::lx::Vector3f trace(
            death::Scene* scene,
            RayHierarchy* out_rays,
            bool debug_camera = false);

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class LightPathImpl;
    LightPathImpl* m_impl;
};

} // namespace cpu
} // namespace death

#endif
