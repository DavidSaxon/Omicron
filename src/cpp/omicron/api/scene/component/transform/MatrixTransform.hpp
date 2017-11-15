/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_SCENE_COMPONENT_TRANSFORM_MATRIXTRANSFORM_HPP_
#define OMICRON_API_SCENE_COMPONENT_TRANSFORM_MATRIXTRANSFORM_HPP_

#include "omicron/api/API.hpp"
#include "omicron/api/scene/component/transform/AbstractTransform.hpp"


namespace omi
{
namespace scene
{

/*!
 * \brief A component which represents its transform using a 4x4 floating point
 *        matrix.
 */
class MatrixTransform
    : public omi::scene::AbstractTransform
{
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Initialises the matrix as an identity matrix.
     *
     * \param constraint The transform this component will be constrained to.
     * \param constraint_type The method that will be used to constrain this
     *                        transform.
     */
    OMI_API_EXPORT MatrixTransform(
            const AbstractTransform* constraint = nullptr,
            ConstraintType constraint_type = kConstraintSRT);

    /*!
     * \brief Initialises this transform with the given matrix.
     *
     * \param matrix The matrix to use as the initial value of this transform.
     * \param constraint The transform this component will be constrained to.
     * \param constraint_type The method that will be used to constrain this
     *                        transform.
     */
    OMI_API_EXPORT MatrixTransform(
            const arc::lx::Matrix44f& matrix,
            const AbstractTransform* constraint = nullptr,
            ConstraintType constraint_type = kConstraintSRT);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual ~MatrixTransform();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual TransformType get_transform_type() const override;

    OMI_API_EXPORT virtual arc::lx::Matrix44f eval() const override;

    /*!
     * \brief Returns a const reference to the matrix of this transform.
     */
    OMI_API_EXPORT const arc::lx::Matrix44f& matrix() const;

    /*!
     * \brief Returns a reference to the matrix of this transform.
     */
    OMI_API_EXPORT arc::lx::Matrix44f& matrix();

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class MatrixTransformImpl;
    MatrixTransformImpl* m_impl;
};

} // namespace scene
} // namespace omi

#endif
