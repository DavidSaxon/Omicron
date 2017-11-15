/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_SCENE_COMPONENT_TRANSFORM_TRANSLATETRANSFORM_HPP_
#define OMICRON_API_SCENE_COMPONENT_TRANSFORM_TRANSLATETRANSFORM_HPP_

#include <arcanecore/lx/Vector.hpp>

#include "omicron/api/API.hpp"
#include "omicron/api/scene/component/transform/AbstractTransform.hpp"


namespace omi
{
namespace scene
{

/*!
 * \brief A component which represents a translation transform.
 */
class TranslateTransform
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
    OMI_API_EXPORT TranslateTransform(
            const AbstractTransform* constraint = nullptr,
            ConstraintType constraint_type = kConstraintSRT);

    /*!
     * \brief Initialises the transform with the given translation values.
     *
     * \param translation The initial values to use as this transform's
     *                    translation.
     * \param constraint The transform this component will be constrained to.
     * \param constraint_type The method that will be used to constrain this
     *                        transform.
     */
    OMI_API_EXPORT TranslateTransform(
            const arc::lx::Vector3f& translation,
            const AbstractTransform* constraint = nullptr,
            ConstraintType constraint_type = kConstraintSRT);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual ~TranslateTransform();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual TransformType get_transform_type() const override;

    OMI_API_EXPORT virtual arc::lx::Matrix44f eval() const override;

    /*!
     * \brief Returns a const reference to the translation of this transform.
     */
    OMI_API_EXPORT const arc::lx::Vector3f& translation() const;

    /*!
     * \brief Returns a reference to the translation of this transform.
     */
    OMI_API_EXPORT arc::lx::Vector3f& translation();

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class TranslateTransformImpl;
    TranslateTransformImpl* m_impl;
};

} // namespace scene
} // namespace omi

#endif
