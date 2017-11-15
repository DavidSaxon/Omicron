/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_SCENE_COMPONENT_TRANSFORM_AXISANGLETRANSFORM_HPP_
#define OMICRON_API_SCENE_COMPONENT_TRANSFORM_AXISANGLETRANSFORM_HPP_

#include <arcanecore/lx/Vector.hpp>

#include "omicron/api/API.hpp"
#include "omicron/api/scene/component/transform/AbstractTransform.hpp"


namespace omi
{
namespace scene
{

/*!
 * \brief A component which represents an axis angle rotation transform.
 */
class AxisAngleTransform
    : public omi::scene::AbstractTransform
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Initialises the transform with no rotation.
     *
     * \param constraint The transform this component will be constrained to.
     * \param constraint_type The method that will be used to constrain this
     *                        transform.
     */
    OMI_API_EXPORT AxisAngleTransform(
            const AbstractTransform* constraint = nullptr,
            ConstraintType constraint_type = kConstraintSRT);

    /*!
     * \brief Initialises the transform with the given rotation values.
     *
     * \param angle The amount (in degrees) to rotate by.
     * \param axis The initial values to use as this transform's axis to rotate
     *             around.
     * \param constraint The transform this component will be constrained to.
     * \param constraint_type The method that will be used to constrain this
     *                        transform.
     */
    OMI_API_EXPORT AxisAngleTransform(
            float angle,
            const arc::lx::Vector3f& axis,
            const AbstractTransform* constraint = nullptr,
            ConstraintType constraint_type = kConstraintSRT);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual ~AxisAngleTransform();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual TransformType get_transform_type() const override;

    OMI_API_EXPORT virtual arc::lx::Matrix44f eval() const override;

    /*!
     * \brief Returns a const reference to the angle of this transform.
     */
    OMI_API_EXPORT const float& angle() const;

    /*!
     * \brief Returns a reference to the angle of this transform.
     */
    OMI_API_EXPORT float& angle();

    /*!
     * \brief Returns a const reference to the axis of this transform.
     */
    OMI_API_EXPORT const arc::lx::Vector3f& axis() const;

    /*!
     * \brief Returns a reference to the axis of this transform.
     */
    OMI_API_EXPORT arc::lx::Vector3f& axis();

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class AxisAngleTransformImpl;
    AxisAngleTransformImpl* m_impl;
};

} // namespace scene
} // namespace omi

#endif
