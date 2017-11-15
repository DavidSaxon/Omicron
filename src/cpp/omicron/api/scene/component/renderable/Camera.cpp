#include "omicron/api/scene/component/renderable/Camera.hpp"

#include <arcanecore/base/math/MathOperations.hpp>
#include <arcanecore/crypt/hash/FNV.hpp>
#include <arcanecore/lx/Alignment.hpp>

#include "omicron/api/scene/component/transform/AbstractTransform.hpp"


namespace omi
{
namespace scene
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

    // hash
    mutable omi::Hash m_hash;
    mutable bool m_hash_out_of_date;
    // properties
    float m_focal_length;
    arc::lx::Vector2f m_sensor_size;
    arc::lx::Vector2f m_sensor_offset;
    float m_near_clip;
    float m_far_clip;
    const AbstractTransform* m_transform;
    // computed value
    float m_computed_diag_sensor_size;
    float m_computed_hor_fov;
    float m_computed_vert_fov;
    float m_computed_diag_fov;
    float m_computed_aspect_ratio;

public:

    ARC_LX_ALIGNED_NEW

    //--------------------------C O N S T R U C T O R---------------------------

    OMI_API_EXPORT CameraImpl(
            float focal_length,
            const arc::lx::Vector2f& sensor_size,
            const arc::lx::Vector2f& sensor_offset,
            float near_clip,
            float far_clip,
            const AbstractTransform* transform)
        : m_hash_out_of_date         (true)
        , m_focal_length             (focal_length)
        , m_sensor_size              (sensor_size)
        , m_sensor_offset            (sensor_offset)
        , m_near_clip                (near_clip)
        , m_far_clip                 (far_clip)
        , m_transform                (transform)
        , m_computed_diag_sensor_size(0.0F)
        , m_computed_hor_fov         (0.0F)
        , m_computed_vert_fov        (0.0F)
        , m_computed_diag_fov        (0.0F)
        , m_computed_aspect_ratio    (0.0F)
    {
        compute_values();
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~CameraImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    RenderableType get_renderable_type() const
    {
        return omi::scene::RenderableType::kCamera;
    }

    const omi::Hash& get_hash() const
    {
        // re-hash?
        if(m_hash_out_of_date)
        {
            m_hash.part1 = arc::crypt::hash::fnv1a_64(this, sizeof(CameraImpl));
            m_hash.part2 = 0;
            m_hash_out_of_date = false;
        }
        return m_hash;
    }

    float get_focal_length() const
    {
        return m_focal_length;
    }

    void set_focal_length(float focal_length)
    {
        m_focal_length = focal_length;
        compute_values();
        m_hash_out_of_date = true;
    }

    const arc::lx::Vector2f& get_sensor_size() const
    {
        return m_sensor_size;
    }

    void set_sensor_size(const arc::lx::Vector2f& sensor_size)
    {
        m_sensor_size = sensor_size;
        compute_values();
        m_hash_out_of_date = true;
    }

    const arc::lx::Vector2f& get_sensor_offset() const
    {
        return m_sensor_offset;
    }

    void set_sensor_offset(const arc::lx::Vector2f& sensor_offset)
    {
        m_sensor_offset = sensor_offset;
        m_hash_out_of_date = true;
    }

    float get_near_clip() const
    {
        return m_near_clip;
    }

    void set_near_clip(float near_clip)
    {
        m_near_clip = near_clip;
        m_hash_out_of_date = true;
    }

    float get_far_clip() const
    {
        return m_far_clip;
    }

    void set_far_clip(float far_clip)
    {
        m_far_clip = far_clip;
        m_hash_out_of_date = true;
    }

    const AbstractTransform* get_transform() const
    {
        return m_transform;
    }

    void set_transform(const AbstractTransform* transform)
    {
        m_transform = transform;
        m_hash_out_of_date = true;
    }

    float get_diagonal_sensor_size() const
    {
        return m_computed_diag_sensor_size;
    }

    float get_horizontal_fov() const
    {
        return m_computed_hor_fov;
    }

    float get_vertical_fov() const
    {
        return m_computed_vert_fov;
    }

    float get_diagonal_fov() const
    {
        return m_computed_diag_fov;
    }

    float get_aspect_ratio() const
    {
        return m_computed_aspect_ratio;
    }

private:

    //------------P R I V A T E    M E M B E R    F U N C T I O N S-------------

    // updates the computed values of this camera
    void compute_values()
    {
        // diagonal sensor size
        m_computed_diag_sensor_size = std::sqrt(
            (m_sensor_size(0) * m_sensor_size(0)) +
            (m_sensor_size(1) * m_sensor_size(1))
        );

        // horizontal field of view
        m_computed_hor_fov = 2.0F * arc::math::radians_to_degrees(
            std::atan(m_sensor_size(0) / (2.0F * m_focal_length))
        );
        // vertical field of view
        m_computed_vert_fov = 2.0F * arc::math::radians_to_degrees(
            std::atan(m_sensor_size(1) / (2.0F * m_focal_length))
        );
        // diagonal field of view
        m_computed_diag_fov = 2.0F * arc::math::radians_to_degrees(
            std::atan(m_computed_diag_sensor_size / (2.0F * m_focal_length))
        );

        // aspect ratio
        m_computed_aspect_ratio = m_sensor_size(0) / m_sensor_size(1);
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_EXPORT Camera::Camera(
        float focal_length,
        const arc::lx::Vector2f& sensor_size,
        const arc::lx::Vector2f& sensor_offset,
        float near_clip,
        float far_clip,
        const AbstractTransform* transform)
    : m_impl(new CameraImpl(
        focal_length,
        sensor_size,
        sensor_offset,
        near_clip,
        far_clip,
        transform
    ))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT Camera::~Camera()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT RenderableType Camera::get_renderable_type() const
{
    return m_impl->get_renderable_type();
}

OMI_API_EXPORT const omi::Hash& Camera::get_hash() const
{
    return m_impl->get_hash();
}

OMI_API_EXPORT float Camera::get_focal_length() const
{
    return m_impl->get_focal_length();
}

OMI_API_EXPORT void Camera::set_focal_length(float focal_length)
{
    m_impl->set_focal_length(focal_length);
}

OMI_API_EXPORT const arc::lx::Vector2f& Camera::get_sensor_size() const
{
    return m_impl->get_sensor_size();
}

OMI_API_EXPORT void Camera::set_sensor_size(
        const arc::lx::Vector2f& sensor_size)
{
    m_impl->set_sensor_size(sensor_size);
}

OMI_API_EXPORT const arc::lx::Vector2f& Camera::get_sensor_offset() const
{
    return m_impl->get_sensor_offset();
}

OMI_API_EXPORT void Camera::set_sensor_offset(
        const arc::lx::Vector2f& sensor_offset)
{
    m_impl->set_sensor_offset(sensor_offset);
}

OMI_API_EXPORT float Camera::get_near_clip() const
{
    return m_impl->get_near_clip();
}

OMI_API_EXPORT void Camera::set_near_clip(float near_clip)
{
    m_impl->set_near_clip(near_clip);
}

OMI_API_EXPORT float Camera::get_far_clip() const
{
    return m_impl->get_far_clip();
}

OMI_API_EXPORT void Camera::set_far_clip(float far_clip)
{
    m_impl->set_far_clip(far_clip);
}

OMI_API_EXPORT const AbstractTransform* Camera::get_transform() const
{
    return m_impl->get_transform();
}

OMI_API_EXPORT void Camera::set_transform(const AbstractTransform* transform)
{
    m_impl->set_transform(transform);
}

OMI_API_EXPORT float Camera::get_diagonal_sensor_size() const
{
    return m_impl->get_diagonal_sensor_size();
}

OMI_API_EXPORT float Camera::get_horizontal_fov() const
{
    return m_impl->get_horizontal_fov();
}

OMI_API_EXPORT float Camera::get_vertical_fov() const
{
    return m_impl->get_vertical_fov();
}

OMI_API_EXPORT float Camera::get_diagonal_fov() const
{
    return m_impl->get_diagonal_fov();
}

OMI_API_EXPORT float Camera::get_aspect_ratio() const
{
    return m_impl->get_aspect_ratio();
}

} // namespace scene
} // namespace omi
