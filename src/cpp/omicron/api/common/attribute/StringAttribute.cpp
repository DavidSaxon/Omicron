#include "omicron/api/common/attribute/StringAttribute.hpp"

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
static StringAttribute::ArrayType g_empty;

} // namespace anonymous

//------------------------------------------------------------------------------
//                               STATIC ATTRIBUTES
//------------------------------------------------------------------------------

OMI_API_GLOBAL Attribute::Type StringAttribute::kTypeString =
    DataAttribute::kTypeDataBits |
    (typeid(StringAttribute::DataType).hash_code() >> 8);

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL StringAttribute::StringAttribute()
    : DataAttribute(kTypeString, true, new StringStorage(0))
{
}

OMI_API_GLOBAL StringAttribute::StringAttribute(DataType value, bool immutable)
    : DataAttribute(
        kTypeString,
        immutable,
        new StringStorage(&value, (&value) + 1, 0)
    )
{
}

OMI_API_GLOBAL StringAttribute::StringAttribute(
        const ArrayType& values,
        std::size_t tuple_size,
        bool immutable)
    : DataAttribute(
        kTypeString,
        immutable,
        new StringStorage(values.begin(), values.end(), tuple_size)
    )
{
}

OMI_API_GLOBAL StringAttribute::StringAttribute(const Attribute& other)
    : DataAttribute(nullptr)
{
    assign(other);
}

OMI_API_GLOBAL StringAttribute::StringAttribute(const StringAttribute& other)
    : DataAttribute(nullptr)
{
    assign(other);
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_GLOBAL StringAttribute::~StringAttribute()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL arc::str::UTF8String StringAttribute::get_type_string()
{
    return "StringAttribute";
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL StringAttribute::DataType StringAttribute::get_value() const
{
    // valid?
    check_state("get_value() used on an invalid attribute");

    // get the storage
    StringStorage* storage = get_storage<StringStorage>();

    // non-empty?
    if(storage->m_data.empty())
    {
        throw arc::ex::IndexOutOfBoundsError(
            "Failed to get single value from empty StringAttribute"
        );
    }

    // return
    return storage->m_data.front();
}

OMI_API_GLOBAL
const StringAttribute::ArrayType& StringAttribute::get_values() const
{
    // valid?
    check_state("get_values() used on an invalid attribute");

    // get the storage and return
    return get_storage<StringStorage>()->m_data;
}

OMI_API_GLOBAL void StringAttribute::set_value(DataType value)
{
    // valid?
    check_state("set_value() used on an invalid attribute");

    prepare_modifcation();
    StringStorage* storage = get_storage<StringStorage>();
    storage->m_data = {value};
}

OMI_API_GLOBAL void StringAttribute::set_values(const ArrayType& values)
{
    // valid?
    check_state("set_values() used on an invalid attribute");

    prepare_modifcation();
    get_storage<StringStorage>()->m_data = values;
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL bool StringAttribute::check_type(Type type) const
{
    return type == kTypeString;
}

} // namespace omi
