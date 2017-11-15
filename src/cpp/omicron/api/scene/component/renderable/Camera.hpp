/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_SCENE_COMPONENT_RENDERABLE_CAMERA_HPP_
#define OMICRON_API_SCENE_COMPONENT_RENDERABLE_CAMERA_HPP_

#include <arcanecore/lx/Vector.hpp>

#include "omicron/api/API.hpp"
#include "omicron/api/common/Hash.hpp"
#include "omicron/api/scene/component/renderable/AbstractRenderable.hpp"


namespace omi
{
namespace scene
{

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class AbstractTransform;

/*!
 * \brief A component that represents a camera located within the 3D scene.
 */
class Camera
    : public omi::scene::AbstractRenderable
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Constructs a camera from the given physical parameters.
     *
     * \param focal_length The optical distance (in mm) from the point where the
     *                     light converges to the camera's sensor.
     * \param sensor_size The width and height (in mm) of the camera's sensor.
     * \param sensor_offset The offset (in mm) on the camera's imaging plane of
     *                      the sensor.
     * \param near_clip The nearest distance from the camera that objects will
     *                  be visible.
     * \param far_clip The furtherest distance from the camera that objects will
     *                  be visible.
     * \param transform The transform controlling the position of this camera.
     */
    OMI_API_EXPORT Camera(
            float focal_length,
            const arc::lx::Vector2f& sensor_size,
            const arc::lx::Vector2f& sensor_offset,
            float near_clip,
            float far_clip,
            const AbstractTransform* transform = nullptr);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual ~Camera();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual RenderableType get_renderable_type() const override;

    /*!
     * \brief Returns current the 128-bit hash of this camera.
     *
     * This is useful for checking whether the camera has changed since the last
     * time it was inspected.
     */
    OMI_API_EXPORT const omi::Hash& get_hash() const;

    /*!
     * \brief Returns the focal length (in mm) of this camera.
     */
    OMI_API_EXPORT float get_focal_length() const;

    /*!
     * \brief Sets the focal length (in mm) of this camera.
     */
    OMI_API_EXPORT void set_focal_length(float focal_length);

    /*!
     * \brief Returns the sensor size (in mm) of this camera.
     */
    OMI_API_EXPORT const arc::lx::Vector2f& get_sensor_size() const;

    /*!
     * \brief Sets the sensor size (in mm) of this camera.
     */
    OMI_API_EXPORT void set_sensor_size(const arc::lx::Vector2f& sensor_size);

    /*!
     * \brief Returns the sensor offset (in mm) of this camera.
     */
    OMI_API_EXPORT const arc::lx::Vector2f& get_sensor_offset() const;

    /*!
     * \brief Sets the sensor offset (in mm) of this camera.
     */
    OMI_API_EXPORT void set_sensor_offset(
            const arc::lx::Vector2f& sensor_offset);

    /*!
     * \brief Returns the distance of the near clipping plane.
     */
    OMI_API_EXPORT float get_near_clip() const;

    /*!
     * \brief Sets the distance of the near clipping of this camera.
     */
    OMI_API_EXPORT void set_near_clip(float near_clip);

    /*!
     * \brief Returns the distance of the far clipping plane.
     */
    OMI_API_EXPORT float get_far_clip() const;

    /*!
     * \brief Sets the distance of the far clipping of this camera.
     */
    OMI_API_EXPORT void set_far_clip(float far_clip);

    /*!
     * \brief Returns the transform controlling the position of this camera.
     *
     * \note This can be null.
     */
    OMI_API_EXPORT const AbstractTransform* get_transform() const;

    /*!
     * \brief Sets the transform controlling the position of this camera.
     *
     * \note This can be null.
     */
    OMI_API_EXPORT void set_transform(const AbstractTransform* transform);

    /*!
     * \brief Returns the computed diagonal size of this camera's sensor.
     */
    OMI_API_EXPORT float get_diagonal_sensor_size() const;

    /*!
     * \brief Returns the computed horizontal field of view of this camera.
     */
    OMI_API_EXPORT float get_horizontal_fov() const;

    /*!
     * \brief Returns the computed vertical field of view of this camera.
     */
    OMI_API_EXPORT float get_vertical_fov() const;

    /*!
     * \brief Returns the computed diagonal field of view of this camera.
     */
    OMI_API_EXPORT float get_diagonal_fov() const;

    /*!
     * \brief Returns the computed aspect ration of this camera.
     */
    OMI_API_EXPORT float get_aspect_ratio() const;

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class CameraImpl;
    CameraImpl* m_impl;
};

} // namespace scene
} // namespace omi

#endif
