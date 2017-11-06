/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/Camera.hpp"

#include <arcanecore/lx/Alignment.hpp>
#include <arcanecore/lx/MatrixMath44f.hpp>


namespace death
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class Camera::CameraImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the API handle to the camera
    DeathCameraHandle m_handle;

    // the transformation matrix of this camera
    arc::lx::Matrix44f m_transform;
    // the projection matrix of this camera
    arc::lx::Matrix44f m_projection_matrix;
    // camera properties
    DeathFloat m_focal_length;
    arc::lx::Vector2f m_filmback_origin;
    arc::lx::Vector2f m_filmback_size;

public:

    ARC_LX_ALIGNED_NEW;

    //--------------------------C O N S T R U C T O R---------------------------

    CameraImpl(DeathCameraHandle handle)
        : m_handle(handle)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~CameraImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    const arc::lx::Matrix44f& get_transform() const
    {
        return m_transform;
    }

    DeathError set_transform(const arc::lx::Matrix44f& transform)
    {
        // TODO: sanity check here?

        m_transform = transform;
        return kDeathSuccess;
    }

    const arc::lx::Matrix44f& get_projection_matrix() const
    {
        return m_projection_matrix;
    }

    DeathFloat get_focal_length() const
    {
        return m_focal_length;
    }

    const arc::lx::Vector2f& get_filmback_origin() const
    {
        return m_filmback_origin;
    }

    const arc::lx::Vector2f& get_filmback_size() const
    {
        return m_filmback_size;
    }

    DeathError set_properties(
            DeathFloat focal_length,
            const arc::lx::Vector2f& filmback_origin,
            const arc::lx::Vector2f& filmback_size)
    {
        // TODO: sanity check here?

        m_focal_length    = focal_length;
        m_filmback_origin = filmback_origin;
        m_filmback_size   = filmback_size;


        // TODO: do this calculation properly
        float fov = 90.0F;
        float aspect = 1000.0F / 1000.0F;
        float z_near  = 0.0001F;
        float z_far   = 100000.0F;
        m_projection_matrix =
            arc::lx::perspective_44f(fov, aspect, z_near, z_far);

        return kDeathSuccess;
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

Camera::Camera(DeathCameraHandle handle)
    : m_impl(new CameraImpl(handle))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

Camera::~Camera()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

const arc::lx::Matrix44f& Camera::get_transform() const
{
    return m_impl->get_transform();
}

DeathError Camera::set_transform(const arc::lx::Matrix44f& transform)
{
    return m_impl->set_transform(transform);
}

const arc::lx::Matrix44f& Camera::get_projection_matrix() const
{
    return m_impl->get_projection_matrix();
}

DeathFloat Camera::get_focal_length() const
{
    return m_impl->get_focal_length();
}

const arc::lx::Vector2f& Camera::get_filmback_origin() const
{
    return m_impl->get_filmback_origin();
}

const arc::lx::Vector2f& Camera::get_filmback_size() const
{
    return m_impl->get_filmback_size();
}

DeathError Camera::set_properties(
        DeathFloat focal_length,
        const arc::lx::Vector2f& filmback_origin,
        const arc::lx::Vector2f& filmback_size)
{
    return m_impl->set_properties(
        focal_length,
        filmback_origin,
        filmback_size
    );
}

} // namespace death

//------------------------------------------------------------------------------
//                               API IMPLEMENTATION
//------------------------------------------------------------------------------

DEATH_API_EXPORT DeathError death_cam_gen(
        DeathSize count,
        DeathCameraHandle* cameras)
{
    DeathCamera* handles = nullptr;
    try
    {
        handles = new DeathCamera[count];
        for(DeathSize i = 0; i < count; ++i)
        {
            handles[i].impl = new death::Camera(handles + i);
        }
    }
    catch(std::bad_alloc&)
    {
        if(handles != nullptr)
        {
            for(DeathSize i = 0; i < count; ++i)
            {
                if(handles[i].impl != nullptr)
                {
                    delete handles[i].impl;
                }
            }
            delete[] handles;
        }
        return kDeathErrorOutOfMemory;
    }

    *cameras = handles;
    return kDeathSuccess;
}

DEATH_API_EXPORT DeathError death_cam_delete(
        DeathSize count,
        DeathCameraHandle* cameras)
{
    if(cameras == nullptr)
    {
        return kDeathErrorNullHandle;
    }

    for(DeathSize i = 0; i < count; ++i)
    {
        delete cameras[i]->impl;
    }
    delete[] *cameras;
    *cameras = nullptr;

    return kDeathSuccess;
}

DEATH_API_EXPORT DeathError death_cam_set_transform(
        DeathCameraHandle camera,
        DeathFloat* matrix44)
{
    if(camera == nullptr)
    {
        return kDeathErrorNullHandle;
    }

    return camera->impl->set_transform(
        *reinterpret_cast<arc::lx::Matrix44f*>(matrix44)
    );
}

DEATH_API_EXPORT DeathError death_cam_set_properties(
        DeathCameraHandle camera,
        DeathFloat focal_length,
        DeathFloat filmback_origin_x,
        DeathFloat filmback_origin_y,
        DeathFloat filmback_width,
        DeathFloat filmback_height)
{
    if(camera == nullptr)
    {
        return kDeathErrorNullHandle;
    }

    return camera->impl->set_properties(
        focal_length,
        arc::lx::Vector2f(filmback_origin_x, filmback_origin_y),
        arc::lx::Vector2f(filmback_width, filmback_height)
    );
}
