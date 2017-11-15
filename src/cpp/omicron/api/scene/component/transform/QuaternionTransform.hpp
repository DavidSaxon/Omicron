/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_SCENE_COMPONENT_TRANSFORM_QUATERNIONTRANSFORM_HPP_
#define OMICRON_API_SCENE_COMPONENT_TRANSFORM_QUATERNIONTRANSFORM_HPP_

#include <arcanecore/lx/Quaternion.hpp>

#include "omicron/api/API.hpp"
#include "omicron/api/scene/component/transform/AbstractTransform.hpp"


namespace omi
{
namespace scene
{

/*!
 * \brief A component which represents a quaternion rotation transform.
 */
class QuaternionTransform
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
    OMI_API_EXPORT QuaternionTransform(
            const AbstractTransform* constraint = nullptr,
            ConstraintType constraint_type = kConstraintSRT);

    /*!
     * \brief Initialises the transform with the given rotation values.
     *
     * \param quaternion The initial values to use as this transform's
     *                   quaternion.
     * \param constraint The transform this component will be constrained to.
     * \param constraint_type The method that will be used to constrain this
     *                        transform.
     */
    OMI_API_EXPORT QuaternionTransform(
            const arc::lx::Quaternionf& quaternion,
            const AbstractTransform* constraint = nullptr,
            ConstraintType constraint_type = kConstraintSRT);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual ~QuaternionTransform();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual TransformType get_transform_type() const override;

    OMI_API_EXPORT virtual arc::lx::Matrix44f eval() const override;

    /*!
     * \brief Sets the translation value of this transform.
     */
    OMI_API_EXPORT void set_quaternion(const arc::lx::Quaternionf& quaternion);

    /*!
     * \brief Returns a const reference to the quaternion of this transform.
     */
    OMI_API_EXPORT const arc::lx::Quaternionf& quaternion() const;

    /*!
     * \brief Returns a reference to the quaternion of this transform.
     */
    OMI_API_EXPORT arc::lx::Quaternionf& quaternion();

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class QuaternionTransformImpl;
    QuaternionTransformImpl* m_impl;
};

} // namespace scene
} // namespace omi

#endif
