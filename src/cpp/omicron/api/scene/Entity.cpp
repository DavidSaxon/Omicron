#include "omicron/api/scene/Entity.hpp"


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

    const arc::str::UTF8String m_name;

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

} // namespace scene
} // namespace
