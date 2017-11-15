#include "omicron/api/scene/component/transform/ScaleTransform.hpp"

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

class ScaleTransform::ScaleTransformImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the wrapper transform object object
    ScaleTransform* m_self;
    // the scale value
    float m_scale;

public:

    ARC_LX_ALIGNED_NEW;

    //--------------------------C O N S T R U C T O R---------------------------

    ScaleTransformImpl(ScaleTransform* self, float scale)
        : m_self (self)
        , m_scale(scale)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~ScaleTransformImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    TransformType get_transform_type() const
    {
        return omi::scene::TransformType::kScale;
    }

    arc::lx::Matrix44f eval() const
    {
        arc::lx::Matrix44f ret = arc::lx::scale_44f(m_scale);
        m_self->apply_constraints(ret);
        return ret;
    }

    const float& scale() const
    {
        return m_scale;
    }

    float& scale()
    {
        return m_scale;
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_EXPORT ScaleTransform::ScaleTransform(
        const AbstractTransform* constraint,
        ConstraintType constraint_type)
    : AbstractTransform(constraint, constraint_type)
    , m_impl(new ScaleTransformImpl(this, 0.0F))
{
}

OMI_API_EXPORT ScaleTransform::ScaleTransform(
        float scale,
        const AbstractTransform* constraint,
        ConstraintType constraint_type)
    : AbstractTransform(constraint, constraint_type)
    , m_impl           (new ScaleTransformImpl(this, scale))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT ScaleTransform::~ScaleTransform()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT TransformType ScaleTransform::get_transform_type() const
{
    return m_impl->get_transform_type();
}

OMI_API_EXPORT arc::lx::Matrix44f ScaleTransform::eval() const
{
    return m_impl->eval();
}

OMI_API_EXPORT const float& ScaleTransform::scale() const
{
    return m_impl->scale();
}

OMI_API_EXPORT float& ScaleTransform::scale()
{
    return m_impl->scale();
}

} // namespace scene
} // namespace omi
