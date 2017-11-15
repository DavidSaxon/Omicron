/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_SCENE_COMPONENT_TRANSFORM_SCALE3TRANSFORM_HPP_
#define OMICRON_API_SCENE_COMPONENT_TRANSFORM_SCALE3TRANSFORM_HPP_

#include <arcanecore/lx/Vector.hpp>

#include "omicron/api/API.hpp"
#include "omicron/api/scene/component/transform/AbstractTransform.hpp"


namespace omi
{
namespace scene
{

/*!
 * \brief A component which represents a 3-dimensional scale transform.
 */
class Scale3Transform
    : public omi::scene::AbstractTransform
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Initialises the transform with no translation.
     *
     * \param constraint The transform this component will be constrained to.
     * \param constraint_type The method that will be used to constrain this
     *                        transform.
     */
    OMI_API_EXPORT Scale3Transform(
            const AbstractTransform* constraint = nullptr,
            ConstraintType constraint_type = kConstraintSRT);

    /*!
     * \brief Initialises the transform with the given scale values.
     *
     * \param scale The initial values to use as this transform's scale.
     * \param constraint The transform this component will be constrained to.
     * \param constraint_type The method that will be used to constrain this
     *                        transform.
     */
    OMI_API_EXPORT Scale3Transform(
            const arc::lx::Vector3f& scale,
            const AbstractTransform* constraint = nullptr,
            ConstraintType constraint_type = kConstraintSRT);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual ~Scale3Transform();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual TransformType get_transform_type() const override;

    OMI_API_EXPORT virtual arc::lx::Matrix44f eval() const override;

    /*!
     * \brief Returns a const reference to the scale of this transform.
     */
    OMI_API_EXPORT const arc::lx::Vector3f& scale() const;

    /*!
     * \brief Returns a reference to the scale of this transform.
     */
    OMI_API_EXPORT arc::lx::Vector3f& scale();

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class Scale3TransformImpl;
    Scale3TransformImpl* m_impl;
};

} // namespace scene
} // namespace omi

#endif
