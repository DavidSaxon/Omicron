#include "omicron/api/scene/component/transform/QuaternionTransform.hpp"

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

class QuaternionTransform::QuaternionTransformImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the wrapper transform object object
    QuaternionTransform* m_self;
    // the quaternion to use for rotation
    arc::lx::Quaternionf m_quaternion;

public:

    ARC_LX_ALIGNED_NEW;

    //--------------------------C O N S T R U C T O R---------------------------

    QuaternionTransformImpl(
            QuaternionTransform* self,
            const arc::lx::Quaternionf& quaternion)
        : m_self      (self)
        , m_quaternion(quaternion)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~QuaternionTransformImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    TransformType get_transform_type() const
    {
        return omi::scene::TransformType::kQuaternion;
    }

    arc::lx::Matrix44f eval() const
    {
        arc::lx::Matrix44f ret = arc::lx::Matrix44f::Identity();
        ret.block<3, 3>(0, 0) = m_quaternion.toRotationMatrix();
        m_self->apply_constraints(ret);
        return ret;
    }

    const arc::lx::Quaternionf& quaternion() const
    {
        return m_quaternion;
    }

    arc::lx::Quaternionf& quaternion()
    {
        return m_quaternion;
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_EXPORT QuaternionTransform::QuaternionTransform(
        const AbstractTransform* constraint,
        ConstraintType constraint_type)
    : AbstractTransform(constraint, constraint_type)
    , m_impl(new QuaternionTransformImpl(
        this,
        arc::lx::Quaternionf()
    ))
{
}

OMI_API_EXPORT QuaternionTransform::QuaternionTransform(
        const arc::lx::Quaternionf& quaternion,
        const AbstractTransform* constraint,
        ConstraintType constraint_type)
    : AbstractTransform(constraint, constraint_type)
    , m_impl           (new QuaternionTransformImpl(this, quaternion))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT QuaternionTransform::~QuaternionTransform()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT TransformType QuaternionTransform::get_transform_type() const
{
    return m_impl->get_transform_type();
}

OMI_API_EXPORT arc::lx::Matrix44f QuaternionTransform::eval() const
{
    return m_impl->eval();
}

OMI_API_EXPORT
const arc::lx::Quaternionf& QuaternionTransform::quaternion() const
{
    return m_impl->quaternion();
}

OMI_API_EXPORT arc::lx::Quaternionf& QuaternionTransform::quaternion()
{
    return m_impl->quaternion();
}

} // namespace scene
} // namespace omi
