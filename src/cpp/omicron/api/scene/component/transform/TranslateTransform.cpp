#include "omicron/api/scene/component/transform/TranslateTransform.hpp"

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

class TranslateTransform::TranslateTransformImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the wrapper transform object object
    TranslateTransform* m_self;
    // the translation
    arc::lx::Vector3f m_translation;

public:

    ARC_LX_ALIGNED_NEW;

    //--------------------------C O N S T R U C T O R---------------------------

    TranslateTransformImpl(
            TranslateTransform* self,
            const arc::lx::Vector3f& translation)
        : m_self       (self)
        , m_translation(translation)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~TranslateTransformImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    TransformType get_transform_type() const
    {
        return omi::scene::TransformType::kTranslate;
    }

    arc::lx::Matrix44f eval() const
    {
        arc::lx::Matrix44f ret = arc::lx::translate_44f(m_translation);
        m_self->apply_constraints(ret);
        return ret;
    }

    const arc::lx::Vector3f& translation() const
    {
        return m_translation;
    }

    arc::lx::Vector3f& translation()
    {
        return m_translation;
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_EXPORT TranslateTransform::TranslateTransform(
        const AbstractTransform* constraint,
        ConstraintType constraint_type)
    : AbstractTransform(constraint, constraint_type)
    , m_impl(new TranslateTransformImpl(
        this,
        arc::lx::Vector3f(0.0F, 0.0F, 0.0F)
    ))
{
}

OMI_API_EXPORT TranslateTransform::TranslateTransform(
        const arc::lx::Vector3f& translation,
        const AbstractTransform* constraint,
        ConstraintType constraint_type)
    : AbstractTransform(constraint, constraint_type)
    , m_impl           (new TranslateTransformImpl(this, translation))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT TranslateTransform::~TranslateTransform()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT TransformType TranslateTransform::get_transform_type() const
{
    return m_impl->get_transform_type();
}

OMI_API_EXPORT arc::lx::Matrix44f TranslateTransform::eval() const
{
    return m_impl->eval();
}

OMI_API_EXPORT const arc::lx::Vector3f& TranslateTransform::translation() const
{
    return m_impl->translation();
}

OMI_API_EXPORT arc::lx::Vector3f& TranslateTransform::translation()
{
    return m_impl->translation();
}

} // namespace scene
} // namespace omi
