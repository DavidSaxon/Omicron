#include "omicron/api/scene/component/AbstractComponent.hpp"

#include <unordered_set>


namespace omi
{
namespace scene
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class AbstractComponent::AbstractComponentImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the wrapper component object
    AbstractComponent* m_self;
    // the unique id of this component
    ComponentId m_id;
    // the components that are dependent on this component
    mutable std::unordered_set<AbstractComponent*> m_dependent;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    AbstractComponentImpl(AbstractComponent* self)
        : m_self(self)
        , m_id  (0)
    {
        static ComponentId g_compontent_id = 0;
        ++g_compontent_id;
        m_id = g_compontent_id;
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~AbstractComponentImpl()
    {
        // remove this from dependencies
        for(AbstractComponent* dependent : m_dependent)
        {
            dependent->dependency_removed(m_self);
        }
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    ComponentId get_id() const
    {
        return m_id;
    }

    void dependent_added(AbstractComponent* dependent) const
    {
        m_dependent.insert(dependent);
    }

    void dependent_removed(AbstractComponent* dependent) const
    {
        auto f_dependent = m_dependent.find(dependent);
        if(f_dependent != m_dependent.end())
        {
            m_dependent.erase(f_dependent);
        }
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT AbstractComponent::AbstractComponent()
    : m_impl(new AbstractComponentImpl(this))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT AbstractComponent::~AbstractComponent()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT ComponentId AbstractComponent::get_id() const
{
    return m_impl->get_id();;
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT void AbstractComponent::dependent_added(
        AbstractComponent* dependent) const
{
    m_impl->dependent_added(dependent);
}

OMI_API_EXPORT void AbstractComponent::dependent_removed(
        AbstractComponent* dependent) const
{
    m_impl->dependent_removed(dependent);
}

} // namespace scene
} // namespace omi
