#include "omicron/api/common/attribute/Attribute.hpp"


namespace omi
{

//------------------------------------------------------------------------------
//                            PUBLIC STATIC ATTRIBUTES
//------------------------------------------------------------------------------

OMI_API_GLOBAL Attribute::Type Attribute::kTypeNull = 0;

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL Attribute::Attribute()
    : m_def  (new Definition(kTypeNull))
    , m_immutable(true)
{
}

OMI_API_GLOBAL Attribute::Attribute(const Attribute& other)
{
    // TODO: check handles are valid
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_GLOBAL Attribute::~Attribute()
{
    decrease_ref();
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL Attribute::Type Attribute::get_type() const
{
    return m_def->m_type;
}

OMI_API_GLOBAL bool Attribute::is_valid() const
{
    return check_type(m_def->m_type);
}


OMI_API_GLOBAL bool Attribute::is_immutable() const
{
    return m_immutable;
}

OMI_API_GLOBAL void Attribute::assign(const Attribute& other)
{
    //  copy immutability
    m_immutable = other.m_immutable;

    // decrease the reference of the existing definition
    if(m_def != nullptr)
    {
        decrease_ref();
    }

    // use the other definition if it's not null and has a valid type
    if(other.m_def != nullptr && check_type(other.m_def->m_type))
    {
        // assign the new definition and increase the reference count
        m_def = other.m_def;
        increase_ref();
    }
    else
    {
        // assign new null definition
        m_def = new Definition(kTypeNull);
    }
}

//------------------------------------------------------------------------------
//                             PROTECTED CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL Attribute::Attribute(Definition* def, bool immutable)
    : m_def      (def)
    , m_immutable(immutable)
{
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL bool Attribute::check_type(Type type) const
{
    // always valid at this level
    return true;
}

OMI_API_GLOBAL void Attribute::increase_ref()
{
    ++m_def->m_ref_count;
}

OMI_API_GLOBAL void Attribute::decrease_ref()
{
    // delete?
    if(m_def->m_ref_count == 1)
    {
        delete m_def;
        m_def = nullptr;
    }
    else
    {
        --m_def->m_ref_count;
    }
}

} // namespace omi
