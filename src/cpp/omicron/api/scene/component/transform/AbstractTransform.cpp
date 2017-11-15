#include "omicron/api/scene/component/transform/AbstractTransform.hpp"

#include <arcanecore/base/Exceptions.hpp>


namespace omi
{
namespace scene
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class AbstractTransform::AbstractTransformImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the wrapper transform object object
    AbstractTransform* m_self;
    // transform this is constraint to
    const AbstractTransform* m_constraint;
    // the method in which this transform is constrained
    ConstraintType m_constraint_type;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    AbstractTransformImpl(
            AbstractTransform* self,
            const AbstractTransform* constraint,
            ConstraintType constraint_type)
        : m_self           (self)
        , m_constraint     (nullptr)
        , m_constraint_type(constraint_type)
    {
        set_constraint(constraint);
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~AbstractTransformImpl()
    {
        // remove dependent
        if(m_constraint != nullptr)
        {
            m_constraint->dependent_removed(m_self);
        }
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    ComponentType get_component_type() const
    {
        return omi::scene::ComponentType::kTransform;
    }

    void set_constraint(const AbstractTransform* constraint)
    {
        // is there already a constraint?
        if(m_constraint != nullptr)
        {
            // we are no longer dependent on it
            m_constraint->dependent_removed(m_self);
        }
        m_constraint = constraint;
        if(m_constraint != nullptr)
        {
            // we dependent on this new transform
            m_constraint->dependent_added(m_self);
        }
    }

    void set_constraint_type(ConstraintType constraint_type)
    {
        m_constraint_type = constraint_type;
    }

    void dependency_removed(AbstractComponent* component)
    {
        m_constraint = nullptr;
    }

    void apply_constraints(arc::lx::Matrix44f& matrix) const
    {
        // no constraints
        if(m_constraint == nullptr || m_constraint_type == kConstraintNone)
        {
            return;
        }
        if(m_constraint_type == kConstraintSRT)
        {
            matrix = m_constraint->eval() * matrix;
            return;
        }
        // TODO: support more constraints
        throw arc::ex::NotImplementedError(
            "Unimplemented transform constraint"
        );
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_EXPORT AbstractTransform::AbstractTransform(
        const AbstractTransform* constraint,
        ConstraintType constraint_type)
    : AbstractComponent()
    , m_impl(new AbstractTransformImpl(this, constraint, constraint_type))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT AbstractTransform::~AbstractTransform()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT
ComponentType AbstractTransform::get_component_type() const
{
    return m_impl->get_component_type();
}

OMI_API_EXPORT void AbstractTransform::set_constraint(
        const AbstractTransform* constraint)
{
    m_impl->set_constraint(constraint);
}

OMI_API_EXPORT void AbstractTransform::set_constraint_type(
        ConstraintType constraint_type)
{
    m_impl->set_constraint_type(constraint_type);
}

OMI_API_EXPORT void AbstractTransform::apply_constraints(
        arc::lx::Matrix44f& matrix) const
{
    m_impl->apply_constraints(matrix);
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT void AbstractTransform::dependency_removed(
        AbstractComponent* component)
{
    m_impl->dependency_removed(component);
}

} // namespace scene
} // namespace omi
