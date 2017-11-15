/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#ifndef DEATHRAY_IMPL_CAMERA_HPP_
#define DEATHRAY_IMPL_CAMERA_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/lx/Matrix.hpp>
#include <arcanecore/lx/Vector.hpp>

#include "deathray/api/API.h"
#include "deathray/api/Camera.h"


namespace death
{

// TODO: DOC
class Camera
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTOR
    //--------------------------------------------------------------------------

    // TODO: DOC
    Camera(DeathCameraHandle handle);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~Camera();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the transform matrix of this camera.
     */
    const arc::lx::Matrix44f& get_transform() const;

    /*!
     * \brief Implementation of the death_cam_set_transform function.
     */
    DeathError set_transform(const arc::lx::Matrix44f& transform);

    /*!
     * \brief Returns the projection matrix of this camera.
     */
    const arc::lx::Matrix44f& get_projection_matrix() const;

    /*!
     * \brief Returns the focal length of this camera.
     */
    DeathFloat get_focal_length() const;

    /*!
     * \brief Returns the sensor size of this camera.
     */
    const arc::lx::Vector2f& get_sensor_size() const;

    /*!
     * \brief Returns the sensor offset of this camera.
     */
    const arc::lx::Vector2f& get_sensor_offset() const;

    /*!
     * \brief Implementation of the death_cam_set_properties function.
     */
    DeathError set_properties(
            DeathFloat focal_length,
            const arc::lx::Vector2f& sensor_size,
            const arc::lx::Vector2f& sensor_offset);

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class CameraImpl;
    CameraImpl* m_impl;
};

} // namespace death

//------------------------------------------------------------------------------
//                             API STRUCT DEFINITIONS
//------------------------------------------------------------------------------

struct DeathCamera
{
    death::Camera* impl;
    DeathCamera() : impl(nullptr) {};
};

#endif
