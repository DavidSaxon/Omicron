#include "omicron/api/common/attribute/DataAttribute.hpp"


namespace omi
{

//------------------------------------------------------------------------------
//                               STATIC ATTRIBUTES
//------------------------------------------------------------------------------

Attribute::Type DataAttribute::kTypeDataBits = 1;

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL DataAttribute::DataAttribute()
    : Attribute(new DataDefinition(Attribute::kTypeNull, 0))
{
}

OMI_API_GLOBAL DataAttribute::DataAttribute(const Attribute& other)
    : Attribute(nullptr)
{
    assign(other);
}

OMI_API_GLOBAL DataAttribute::DataAttribute(const DataAttribute& other)
    : Attribute(nullptr)
{
    assign(other);
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_GLOBAL DataAttribute::~DataAttribute()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL std::size_t DataAttribute::get_size() const
{
    return static_cast<DataDefinition*>(m_def)->m_size;
}

OMI_API_GLOBAL std::size_t DataAttribute::get_tuple_size() const
{
    return static_cast<DataDefinition*>(m_def)->m_tuple_size;
}

OMI_API_GLOBAL bool DataAttribute::check_type(Type type) const
{
    return type & kTypeDataBits;
}

//------------------------------------------------------------------------------
//                             PROTECTED CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL DataAttribute::DataAttribute(Definition* def, bool immutable)
    : Attribute(def, immutable)
{
}

} // namespace omi
