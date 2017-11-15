#include "omicron/api/scene/component/transform/Scale3Transform.hpp"

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

class Scale3Transform::Scale3TransformImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the wrapper transform object object
    Scale3Transform* m_self;
    // the scale
    arc::lx::Vector3f m_scale;

public:

    ARC_LX_ALIGNED_NEW;

    //--------------------------C O N S T R U C T O R---------------------------

    Scale3TransformImpl(
            Scale3Transform* self,
            const arc::lx::Vector3f& scale)
        : m_self (self)
        , m_scale(scale)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~Scale3TransformImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    TransformType get_transform_type() const
    {
        return omi::scene::TransformType::kScale3;
    }

    arc::lx::Matrix44f eval() const
    {
        arc::lx::Matrix44f ret = arc::lx::scale_44f(m_scale);
        m_self->apply_constraints(ret);
        return ret;
    }

    const arc::lx::Vector3f& scale() const
    {
        return m_scale;
    }

    arc::lx::Vector3f& scale()
    {
        return m_scale;
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_EXPORT Scale3Transform::Scale3Transform(
        const AbstractTransform* constraint,
        ConstraintType constraint_type)
    : AbstractTransform(constraint, constraint_type)
    , m_impl(new Scale3TransformImpl(
        this,
        arc::lx::Vector3f(0.0F, 0.0F, 0.0F)
    ))
{
}

OMI_API_EXPORT Scale3Transform::Scale3Transform(
        const arc::lx::Vector3f& scale,
        const AbstractTransform* constraint,
        ConstraintType constraint_type)
    : AbstractTransform(constraint, constraint_type)
    , m_impl           (new Scale3TransformImpl(this, scale))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT Scale3Transform::~Scale3Transform()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT TransformType Scale3Transform::get_transform_type() const
{
    return m_impl->get_transform_type();
}

OMI_API_EXPORT arc::lx::Matrix44f Scale3Transform::eval() const
{
    return m_impl->eval();
}

OMI_API_EXPORT const arc::lx::Vector3f& Scale3Transform::scale() const
{
    return m_impl->scale();
}

OMI_API_EXPORT arc::lx::Vector3f& Scale3Transform::scale()
{
    return m_impl->scale();
}

} // namespace scene
} // namespace omi
