#include "omicron/api/common/attribute/FloatAttribute.hpp"

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
static FloatAttribute::ArrayType g_empty;

} // namespace anonymous

//------------------------------------------------------------------------------
//                               STATIC ATTRIBUTES
//------------------------------------------------------------------------------

OMI_API_GLOBAL Attribute::Type FloatAttribute::kTypeFloat =
    DataAttribute::kTypeDataBits |
    (typeid(FloatAttribute::DataType).hash_code() >> 8);

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL FloatAttribute::FloatAttribute()
    : DataAttribute(kTypeFloat, true, new FloatStorage(0))
{
}

OMI_API_GLOBAL FloatAttribute::FloatAttribute(DataType value, bool immutable)
    : DataAttribute(
        kTypeFloat,
        immutable,
        new FloatStorage(&value, (&value) + 1, 0)
    )
{
}

OMI_API_GLOBAL FloatAttribute::FloatAttribute(
        const ArrayType& values,
        std::size_t tuple_size,
        bool immutable)
    : DataAttribute(
        kTypeFloat,
        immutable,
        new FloatStorage(values.begin(), values.end(), tuple_size)
    )
{
}

OMI_API_GLOBAL FloatAttribute::FloatAttribute(const Attribute& other)
    : DataAttribute(nullptr)
{
    assign(other);
}

OMI_API_GLOBAL FloatAttribute::FloatAttribute(const FloatAttribute& other)
    : DataAttribute(nullptr)
{
    assign(other);
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_GLOBAL FloatAttribute::~FloatAttribute()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL arc::str::UTF8String FloatAttribute::get_type_string()
{
    return "FloatAttribute";
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL FloatAttribute::DataType FloatAttribute::get_value() const
{
    // valid?
    check_state("get_value() used on an invalid attribute");

    // get the storage
    FloatStorage* storage = get_storage<FloatStorage>();

    // non-empty?
    if(storage->m_data.empty())
    {
        throw arc::ex::IndexOutOfBoundsError(
            "Failed to get single value from empty FloatAttribute"
        );
    }

    // FloatAttribute
    return storage->m_data.front();
}

OMI_API_GLOBAL
const FloatAttribute::ArrayType& FloatAttribute::get_values() const
{
    // valid?
    check_state("get_values() used on an invalid attribute");

    // get the storage and return
    return get_storage<FloatStorage>()->m_data;
}

OMI_API_GLOBAL const FloatAttribute::DataType& FloatAttribute::at(
        std::size_t index) const
{
    // valid?
    check_state("at() used on an invalid attribute");

    // get the storage
    FloatStorage* storage = get_storage<FloatStorage>();

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

OMI_API_GLOBAL void FloatAttribute::set_value(DataType value)
{
    // valid?
    check_state("set_value() used on an invalid attribute");

    prepare_modifcation();
    FloatStorage* storage = get_storage<FloatStorage>();
    storage->m_data = {value};
}

OMI_API_GLOBAL void FloatAttribute::set_values(const ArrayType& values)
{
    // valid?
    check_state("set_values() used on an invalid attribute");

    prepare_modifcation();
    get_storage<FloatStorage>()->m_data = values;
}

OMI_API_GLOBAL void FloatAttribute::set_at(
        std::size_t index,
        DataType value)
{
    // valid?
    check_state("at() used on an invalid attribute");

    // get the storage
    FloatStorage* storage = get_storage<FloatStorage>();

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

OMI_API_GLOBAL bool FloatAttribute::check_type(Type type) const
{
    return type == kTypeFloat;
}

} // namespace omi
