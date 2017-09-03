#include "omicron/api/common/attribute/ByteAttribute.hpp"

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
static ByteAttribute::ArrayType g_empty;

} // namespace anonymous

//------------------------------------------------------------------------------
//                               STATIC ATTRIBUTES
//------------------------------------------------------------------------------

OMI_API_GLOBAL Attribute::Type ByteAttribute::kTypeByte =
    DataAttribute::kTypeDataBits |
    (typeid(ByteAttribute::DataType).hash_code() >> 8);

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL ByteAttribute::ByteAttribute()
    : DataAttribute(kTypeByte, true, new ByteStorage(0))
{
}

OMI_API_GLOBAL ByteAttribute::ByteAttribute(DataType value, bool immutable)
    : DataAttribute(
        kTypeByte,
        immutable,
        new ByteStorage(&value, (&value) + 1, 0)
    )
{
}

OMI_API_GLOBAL ByteAttribute::ByteAttribute(
        const ArrayType& values,
        std::size_t tuple_size,
        bool immutable)
    : DataAttribute(
        kTypeByte,
        immutable,
        new ByteStorage(values.begin(), values.end(), tuple_size)
    )
{
}

OMI_API_GLOBAL ByteAttribute::ByteAttribute(const Attribute& other)
    : DataAttribute(nullptr)
{
    assign(other);
}

OMI_API_GLOBAL ByteAttribute::ByteAttribute(const ByteAttribute& other)
    : DataAttribute(nullptr)
{
    assign(other);
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_GLOBAL ByteAttribute::~ByteAttribute()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL arc::str::UTF8String ByteAttribute::get_type_string()
{
    return "ByteAttribute";
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL ByteAttribute::DataType ByteAttribute::get_value() const
{
    // valid?
    check_state("get_value() used on an invalid attribute");

    // get the storage
    ByteStorage* storage = get_storage<ByteStorage>();

    // non-empty?
    if(storage->m_data.empty())
    {
        throw arc::ex::IndexOutOfBoundsError(
            "Failed to get single value from empty ByteAttribute"
        );
    }

    // return
    return storage->m_data.front();
}

OMI_API_GLOBAL
const ByteAttribute::ArrayType& ByteAttribute::get_values() const
{
    // valid?
    check_state("get_values() used on an invalid attribute");

    // get the storage and return
    return get_storage<ByteStorage>()->m_data;
}

OMI_API_GLOBAL void ByteAttribute::set_value(DataType value)
{
    // valid?
    check_state("set_value() used on an invalid attribute");

    prepare_modifcation();
    ByteStorage* storage = get_storage<ByteStorage>();
    storage->m_data = {value};
}

OMI_API_GLOBAL void ByteAttribute::set_values(const ArrayType& values)
{
    // valid?
    check_state("set_values() used on an invalid attribute");

    prepare_modifcation();
    get_storage<ByteStorage>()->m_data = values;
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL bool ByteAttribute::check_type(Type type) const
{
    return type == kTypeByte;
}

} // namespace omi
