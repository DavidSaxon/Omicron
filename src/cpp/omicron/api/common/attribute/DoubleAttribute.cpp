#include "omicron/api/common/attribute/DoubleAttribute.hpp"

#include <typeinfo>

#include <arcanecore/base/Exceptions.hpp>


namespace omi
{

//------------------------------------------------------------------------------
//                                    GLOBALS
//------------------------------------------------------------------------------

namespace
{

// used for returning references to empty data
static DoubleAttribute::ArrayType g_empty;

} // namespace anonymous

//------------------------------------------------------------------------------
//                               STATIC ATTRIBUTES
//------------------------------------------------------------------------------

OMI_API_GLOBAL Attribute::Type DoubleAttribute::kTypeDouble =
    DataAttribute::kTypeDataBits |
    (typeid(DoubleAttribute::DataType).hash_code() >> 8);

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL DoubleAttribute::DoubleAttribute()
    : DataAttribute(kTypeDouble, true, new DoubleStorage(0))
{
}

OMI_API_GLOBAL DoubleAttribute::DoubleAttribute(DataType value, bool immutable)
    : DataAttribute(
        kTypeDouble,
        immutable,
        new DoubleStorage(&value, (&value) + 1, 0)
    )
{
}

OMI_API_GLOBAL DoubleAttribute::DoubleAttribute(
        const ArrayType& values,
        std::size_t tuple_size,
        bool immutable)
    : DataAttribute(
        kTypeDouble,
        immutable,
        new DoubleStorage(values.begin(), values.end(), tuple_size)
    )
{
}

OMI_API_GLOBAL DoubleAttribute::DoubleAttribute(const Attribute& other)
    : DataAttribute(nullptr)
{
    assign(other);
}

OMI_API_GLOBAL DoubleAttribute::DoubleAttribute(const DoubleAttribute& other)
    : DataAttribute(nullptr)
{
    assign(other);
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_GLOBAL DoubleAttribute::~DoubleAttribute()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL arc::str::UTF8String DoubleAttribute::get_type_string()
{
    return "DoubleAttribute";
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL DoubleAttribute::DataType DoubleAttribute::get_value() const
{
    // valid?
    check_state("get_value() used on an invalid attribute");

    // get the storage
    DoubleStorage* storage = get_storage<DoubleStorage>();

    // non-empty?
    if(storage->m_data.empty())
    {
        throw arc::ex::IndexOutOfBoundsError(
            "Failed to get single value from empty DoubleAttribute"
        );
    }

    // return
    return storage->m_data.front();
}

OMI_API_GLOBAL
const DoubleAttribute::ArrayType& DoubleAttribute::get_values() const
{
    // valid?
    check_state("get_values() used on an invalid attribute");

    // get the storage and return
    return get_storage<DoubleStorage>()->m_data;
}

OMI_API_GLOBAL const DoubleAttribute::DataType& DoubleAttribute::at(
        std::size_t index) const
{
    // valid?
    check_state("at() used on an invalid attribute");

    // get the storage
    DoubleStorage* storage = get_storage<DoubleStorage>();

    // check bounds
    if(index >= storage->m_data.size())
    {
        arc::str::UTF8String error_message;
        error_message
            << "Index: " << index << " is greater than or equal to attribute's "
            << "data size.";
        throw arc::ex::IndexOutOfBoundsError(error_message);
    }

    return storage->m_data[index];
}

OMI_API_GLOBAL void DoubleAttribute::set_value(DataType value)
{
    // valid?
    check_state("set_value() used on an invalid attribute");

    prepare_modifcation();
    DoubleStorage* storage = get_storage<DoubleStorage>();
    storage->m_data = {value};
}

OMI_API_GLOBAL void DoubleAttribute::set_values(const ArrayType& values)
{
    // valid?
    check_state("set_values() used on an invalid attribute");

    prepare_modifcation();
    get_storage<DoubleStorage>()->m_data = values;
}

OMI_API_GLOBAL void DoubleAttribute::set_at(
        std::size_t index,
        DataType value)
{
    // valid?
    check_state("at() used on an invalid attribute");

    // get the storage
    DoubleStorage* storage = get_storage<DoubleStorage>();

    // check bounds
    if(index >= storage->m_data.size())
    {
        arc::str::UTF8String error_message;
        error_message
            << "Index: " << index << " is greater than or equal to attribute's "
            << "data size.";
        throw arc::ex::IndexOutOfBoundsError(error_message);
    }

    // soft modification
    prepare_modifcation(true);

    storage->m_data[index] = value;
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL bool DoubleAttribute::check_type(Type type) const
{
    return type == kTypeDouble;
}

} // namespace omi
