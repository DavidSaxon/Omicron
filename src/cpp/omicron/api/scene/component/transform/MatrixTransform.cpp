#include "omicron/api/scene/component/transform/MatrixTransform.hpp"

#include <arcanecore/lx/Alignment.hpp>

#include "omicron/api/scene/component/transform/AbstractTransform.hpp"


namespace omi
{
namespace scene
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class MatrixTransform::MatrixTransformImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the wrapper transform object object
    MatrixTransform* m_self;
    // the 4x4 floating point matrix
    arc::lx::Matrix44f m_matrix;

public:

    ARC_LX_ALIGNED_NEW;

    //--------------------------C O N S T R U C T O R---------------------------

    MatrixTransformImpl(MatrixTransform* self, const arc::lx::Matrix44f& matrix)
        : m_self  (self)
        , m_matrix(matrix)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~MatrixTransformImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    TransformType get_transform_type() const
    {
        return omi::scene::TransformType::kMatrix;
    }

    arc::lx::Matrix44f eval() const
    {
        arc::lx::Matrix44f ret = m_matrix;
        m_self->apply_constraints(ret);
        return ret;
    }

    const arc::lx::Matrix44f& matrix() const
    {
        return m_matrix;
    }

    arc::lx::Matrix44f& matrix()
    {
        return m_matrix;
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_EXPORT MatrixTransform::MatrixTransform(
        const AbstractTransform* constraint,
        ConstraintType constraint_type)
    : AbstractTransform(constraint, constraint_type)
    , m_impl(new MatrixTransformImpl(this, arc::lx::Matrix44f::Identity()))
{
}

OMI_API_EXPORT MatrixTransform::MatrixTransform(
        const arc::lx::Matrix44f& matrix,
        const AbstractTransform* constraint,
        ConstraintType constraint_type)
    : AbstractTransform(constraint, constraint_type)
    , m_impl           (new MatrixTransformImpl(this, matrix))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT MatrixTransform::~MatrixTransform()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT TransformType MatrixTransform::get_transform_type() const
{
    return m_impl->get_transform_type();
}

OMI_API_EXPORT arc::lx::Matrix44f MatrixTransform::eval() const
{
    return m_impl->eval();
}

OMI_API_EXPORT const arc::lx::Matrix44f& MatrixTransform::matrix() const
{
    return m_impl->matrix();
}

OMI_API_EXPORT arc::lx::Matrix44f& MatrixTransform::matrix()
{
    return m_impl->matrix();
}

} // namespace scene
} // namespace omi
