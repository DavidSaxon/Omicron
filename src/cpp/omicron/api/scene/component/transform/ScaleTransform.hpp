/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_SCENE_COMPONENT_TRANSFORM_SCALETRANSFORM_HPP_
#define OMICRON_API_SCENE_COMPONENT_TRANSFORM_SCALETRANSFORM_HPP_

#include <arcanecore/lx/Vector.hpp>

#include "omicron/api/API.hpp"
#include "omicron/api/scene/component/transform/AbstractTransform.hpp"


namespace omi
{
namespace scene
{

/*!
 * \brief A component which represents a uniform scale transform.
 */
class ScaleTransform
    : public omi::scene::AbstractTransform
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Initialises the transform with no scaling.
     *
     * \param constraint The transform this component will be constrained to.
     * \param constraint_type The method that will be used to constrain this
     *                        transform.
     */
    OMI_API_EXPORT ScaleTransform(
            const AbstractTransform* constraint = nullptr,
            ConstraintType constraint_type = kConstraintSRT);

    /*!
     * \brief Initialises the transform with the given scale amount.
     *
     * \param scale The initial value to use as this transform's scale.
     * \param constraint The transform this component will be constrained to.
     * \param constraint_type The method that will be used to constrain this
     *                        transform.
     */
    OMI_API_EXPORT ScaleTransform(
            float scale,
            const AbstractTransform* constraint = nullptr,
            ConstraintType constraint_type = kConstraintSRT);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual ~ScaleTransform();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual TransformType get_transform_type() const override;

    OMI_API_EXPORT virtual arc::lx::Matrix44f eval() const override;

    /*!
     * \brief Returns a const reference to the scale of this transform.
     */
    OMI_API_EXPORT const float& scale() const;

    /*!
     * \brief Returns a reference to the scale of this transform.
     */
    OMI_API_EXPORT float& scale();

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class ScaleTransformImpl;
    ScaleTransformImpl* m_impl;
};

} // namespace scene
} // namespace omi

#endif
