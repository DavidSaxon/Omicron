#include "omicron/api/scene/component/transform/AxisAngleTransform.hpp"

#include <arcanecore/lx/Alignment.hpp>
#include <arcanecore/lx/MatrixMath44f.hpp>

#include "omicron/api/scene/component/transform/AbstractTransform.hpp"


namespace omi
{
namespace scene
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class AxisAngleTransform::AxisAngleTransformImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the wrapper transform object object
    AxisAngleTransform* m_self;
    // the angle to rotate by
    float m_angle;
    // the axis to rotate around
    arc::lx::Vector3f m_axis;

public:

    ARC_LX_ALIGNED_NEW;

    //--------------------------C O N S T R U C T O R---------------------------

    AxisAngleTransformImpl(
            AxisAngleTransform* self,
            float angle,
            const arc::lx::Vector3f& axis)
        : m_self (self)
        , m_angle(angle)
        , m_axis (axis)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~AxisAngleTransformImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    TransformType get_transform_type() const
    {
        return omi::scene::TransformType::kAxisAngle;
    }

    arc::lx::Matrix44f eval() const
    {
        arc::lx::Matrix44f ret = arc::lx::rotate_axis_44f(m_angle, m_axis);
        m_self->apply_constraints(ret);
        return ret;
    }

    const float& angle() const
    {
        return m_angle;
    }

    float& angle()
    {
        return m_angle;
    }

    const arc::lx::Vector3f& axis() const
    {
        return m_axis;
    }

    arc::lx::Vector3f& axis()
    {
        return m_axis;
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_EXPORT AxisAngleTransform::AxisAngleTransform(
        const AbstractTransform* constraint,
        ConstraintType constraint_type)
    : AbstractTransform(constraint, constraint_type)
    , m_impl(new AxisAngleTransformImpl(
        this,
        0.0F,
        arc::lx::Vector3f(0.0F, 0.0F, 0.0F)
    ))
{
}

OMI_API_EXPORT AxisAngleTransform::AxisAngleTransform(
        float angle,
        const arc::lx::Vector3f& axis,
        const AbstractTransform* constraint,
        ConstraintType constraint_type)
    : AbstractTransform(constraint, constraint_type)
    , m_impl           (new AxisAngleTransformImpl(this, angle, axis))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT AxisAngleTransform::~AxisAngleTransform()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT TransformType AxisAngleTransform::get_transform_type() const
{
    return m_impl->get_transform_type();
}

OMI_API_EXPORT arc::lx::Matrix44f AxisAngleTransform::eval() const
{
    return m_impl->eval();
}

OMI_API_EXPORT const float& AxisAngleTransform::angle() const
{
    return m_impl->angle();
}

OMI_API_EXPORT float& AxisAngleTransform::angle()
{
    return m_impl->angle();
}

OMI_API_EXPORT const arc::lx::Vector3f& AxisAngleTransform::axis() const
{
    return m_impl->axis();
}

OMI_API_EXPORT arc::lx::Vector3f& AxisAngleTransform::axis()
{
    return m_impl->axis();
}

} // namespace scene
} // namespace omi
