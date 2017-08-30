#include "omicron/api/common/attribute/DataAttribute.hpp"

#include <arcanecore/base/Exceptions.hpp>


namespace omi
{

//------------------------------------------------------------------------------
//                               STATIC ATTRIBUTES
//------------------------------------------------------------------------------

Attribute::Type DataAttribute::kTypeDataBits = 1;

//------------------------------------------------------------------------------
//                                    STORAGE
//------------------------------------------------------------------------------

//----------------------------C O N S T R U C T O R-----------------------------

OMI_API_GLOBAL DataAttribute::DataStorage::DataStorage(std::size_t tuple_size)
    : m_tuple_size(tuple_size)
{
}

//-----------------------------D E S T R U C T O R------------------------------

OMI_API_GLOBAL DataAttribute::DataStorage::~DataStorage()
{
}

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL DataAttribute::DataAttribute()
    : Attribute(kTypeNull, true, nullptr)
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
    return get_storage<DataStorage>()->get_size();
}

OMI_API_GLOBAL std::size_t DataAttribute::get_tuple_size() const
{
    return get_storage<DataStorage>()->m_tuple_size;
}

OMI_API_GLOBAL void DataAttribute::set_tuple_size(std::size_t tuple_size)
{
    if(!is_valid())
    {
        throw arc::ex::StateError("Invalid DataAttribute");
    }
    // soft modification
    prepare_modifcation(true);
    get_storage<DataStorage>()->m_tuple_size = tuple_size;
}

//------------------------------------------------------------------------------
//                             PROTECTED CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL DataAttribute::DataAttribute(Definition* def)
    : Attribute(def)
{
}

OMI_API_GLOBAL DataAttribute::DataAttribute(
        Type type,
        bool immutable,
        Storage* storage)
    : Attribute(type, immutable, storage)
{
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL bool DataAttribute::check_type(Type type) const
{
    return type & kTypeDataBits;
}

} // namespace omi
