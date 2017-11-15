/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_SCENE_COMPONENT_TRANSFORM_ABSTRACTTRANSFORM_HPP_
#define OMICRON_API_SCENE_COMPONENT_TRANSFORM_ABSTRACTTRANSFORM_HPP_

#include <arcanecore/lx/Matrix.hpp>

#include "omicron/api/API.hpp"
#include "omicron/api/scene/component/AbstractComponent.hpp"


namespace omi
{
namespace scene
{

//------------------------------------------------------------------------------
//                                     ENUMS
//------------------------------------------------------------------------------

/*!
 * \brief Defines the different derived transform types.
 */
enum class TransformType
{
    kMatrix,
    kTranslate,
    kAxisAngle,
    kQuaternion,
    kScale,
    kScale3
};


/*!
 * \brief A component that defines a 3D transformation which can be attached to
 *        other component types.
 */
class AbstractTransform
    : public omi::scene::AbstractComponent
{
public:

    //--------------------------------------------------------------------------
    //                                   ENUMS
    //--------------------------------------------------------------------------

    enum ConstraintType
    {
        kConstraintNone      = 0UL,
        kConstraintScale     = 1UL << 0,
        kConstraintRotateX   = 1UL << 1,
        kConstraintRotateY   = 1UL << 2,
        kConstraintRotateZ   = 1UL << 3,
        kConstraintTranslate = 1UL << 4,
        kConstraintRotateXYZ =
            kConstraintRotateX | kConstraintRotateY | kConstraintRotateZ,
        kConstraintSRT       =
            kConstraintScale | kConstraintRotateXYZ | kConstraintTranslate
        // TODO: custom constraints
    };

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Creates a new transform component with the given parent that this
     *        will be constrained to (can be null).
     */
    OMI_API_EXPORT AbstractTransform(
            const AbstractTransform* constraint = nullptr,
            ConstraintType constraint_type = kConstraintSRT);

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual ~AbstractTransform();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual ComponentType get_component_type() const override;

    /*!
     * \brief Returns the transform type of this component.
     */
    virtual TransformType get_transform_type() const = 0;

    /*!
     * \brief Returns the evaluated matrix of this transform (including
     *        constraints).
     */
    virtual arc::lx::Matrix44f eval() const = 0;

    /*!
     * \brief Sets the transform this will be constrained to.
     */
    OMI_API_EXPORT void set_constraint(const AbstractTransform* constraint);

    /*!
     * \brief Sets the method this transform will be using for constraining.
     */
    OMI_API_EXPORT void set_constraint_type(ConstraintType constraint_type);

    /*!
     * \brief Applies this transform's constraints to the given 4x4
     *        transformation matrix.
     */
    OMI_API_EXPORT void apply_constraints(arc::lx::Matrix44f& matrix) const;

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    OMI_API_EXPORT virtual void dependency_removed(
            AbstractComponent* component) override;

private:

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class AbstractTransformImpl;
    AbstractTransformImpl* m_impl;
};

} // namespace scene
} // namespace omi

#endif
