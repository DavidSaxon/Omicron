#include "omicron/api/scene/Entity.hpp"

#include <arcanecore/base/Exceptions.hpp>

#include "omicron/api/scene/SceneGlobals.hpp"
#include "omicron/api/scene/component/AbstractComponent.hpp"


namespace omi
{
namespace scene
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class Entity::EntityImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // the name of this entity
    const arc::str::UTF8String m_name;

    // the lists of the various components of this entity
    std::list<AbstractComponent*> m_components;

    // the components that are new for this entity
    std::list<AbstractComponent*> m_new_components;
    // the components that have been removed from this entity
    std::list<AbstractComponent*> m_removed_components;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    EntityImpl(const arc::str::UTF8String& name)
        : m_name(name)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~EntityImpl()
    {
        #ifndef OMI_API_MODE_PRODUCTION
            if(!m_components.empty())
            {
                global::logger->warning
                    << "Entity: \"" << get_name() << "\" had remaining "
                    << "components at destruction - these should have been "
                    << "handled by the engine." << std::endl;
            }
            if(!m_removed_components.empty())
            {
                global::logger->warning
                    << "Entity: \"" << get_name() << "\" had remaining removed "
                    << "components at destruction - these should have been "
                    << "handled by the engine." << std::endl;
            }
        #endif

        // delete any remaining removed components
        for(AbstractComponent* component : m_removed_components)
        {
            delete component;
        }
        // delete any remaining components
        for(AbstractComponent* component : m_components)
        {
            delete component;
        }
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    const arc::str::UTF8String& get_name() const
    {
        return m_name;
    }

    const std::list<AbstractComponent*>& get_components() const
    {
        return m_components;
    }

    void add_component(AbstractComponent* component)
    {
        // check for duplicates
        #ifndef OMI_API_MODE_PRODUCTION
            for(AbstractComponent* c : m_components)
            {
                if(component == c)
                {
                    throw arc::ex::ValueError(
                        "Duplicate component added to entity"
                    );
                }
            }
        #endif

        m_components.push_back(component);
        m_new_components.push_back(component);
    }

    std::vector<AbstractComponent*> retrieve_new_components()
    {
        std::vector<AbstractComponent*> ret(
            m_new_components.begin(),
            m_new_components.end()
        );
        m_new_components.clear();
        return ret;
    }

    std::vector<AbstractComponent*> retrieve_removed_components()
    {
        std::vector<AbstractComponent*> ret(
            m_removed_components.begin(),
            m_removed_components.end()
        );
        m_removed_components.clear();
        return ret;
    }
};

//------------------------------------------------------------------------------
//                                  CONSTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT Entity::Entity(const arc::str::UTF8String& name)
    : m_impl(new EntityImpl(name))
{
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT Entity::~Entity()
{
    delete m_impl;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT const arc::str::UTF8String& Entity::get_name() const
{
    return m_impl->get_name();
}

OMI_API_EXPORT
const std::list<AbstractComponent*>& Entity::get_components() const
{
    return m_impl->get_components();
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT void Entity::add_component(AbstractComponent* component)
{
    m_impl->add_component(component);
}

OMI_API_EXPORT
std::vector<AbstractComponent*> Entity::retrieve_new_components()
{
    return m_impl->retrieve_new_components();
}

OMI_API_EXPORT
std::vector<AbstractComponent*> Entity::retrieve_removed_components()
{
    return m_impl->retrieve_removed_components();
}

} // namespace scene
} // namespace
