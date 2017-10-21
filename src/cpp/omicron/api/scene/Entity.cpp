#include "omicron/api/scene/Entity.hpp"

#include <arcanecore/base/Exceptions.hpp>

#include "omicron/api/scene/component/AbstractComponent.hpp"
#include "omicron/api/scene/component/renderable/AbstractRenderable.hpp"


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
    std::list<AbstractRenderable*> m_renderables;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    EntityImpl(const arc::str::UTF8String& name)
        : m_name(name)
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~EntityImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    const arc::str::UTF8String& get_name() const
    {
        return m_name;
    }

    const std::list<AbstractRenderable*>& get_renderable_components() const
    {
        return m_renderables;
    }

    void add_component(AbstractComponent* component)
    {
        // determine the type of the component
        switch(component->get_component_type())
        {
            case omi::scene::ComponentType::kRenderable:
            {
                AbstractRenderable* renderable =
                    static_cast<AbstractRenderable*>(component);

                // check for duplicates
                #ifndef OMI_API_MODE_PRODUCTION
                    for(AbstractRenderable* renderable : m_renderables)
                    {
                        if(renderable == renderable)
                        {
                            throw arc::ex::ValueError(
                                "Duplicate renderable component added to entity"
                            );
                        }
                    }
                #endif

                m_renderables.push_back(renderable);
                break;
            }
            default:
            {
                arc::str::UTF8String error_message;
                error_message
                    << "Component of unknown type added: "
                    << static_cast<int>(component->get_component_type());
                throw arc::ex::TypeError(error_message);
            }
        }
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
const std::list<AbstractRenderable*>& Entity::get_renderable_components() const
{
    return m_impl->get_renderable_components();
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT void Entity::add_component(AbstractComponent* component)
{
    m_impl->add_component(component);
}

} // namespace scene
} // namespace
