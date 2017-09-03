#include "omicron/api/common/attribute/BoolAttribute.hpp"

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
static BoolAttribute::ArrayType g_empty;

} // namespace anonymous

//------------------------------------------------------------------------------
//                               STATIC ATTRIBUTES
//------------------------------------------------------------------------------

OMI_API_GLOBAL Attribute::Type BoolAttribute::kTypeBool =
    DataAttribute::kTypeDataBits |
    (typeid(BoolAttribute::DataType).hash_code() >> 8);

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL BoolAttribute::BoolAttribute()
    : DataAttribute(kTypeBool, true, new BoolStorage(0))
{
}

OMI_API_GLOBAL BoolAttribute::BoolAttribute(DataType value, bool immutable)
    : DataAttribute(
        kTypeBool,
        immutable,
        new BoolStorage(&value, (&value) + 1, 0)
    )
{
}

OMI_API_GLOBAL BoolAttribute::BoolAttribute(
        const ArrayType& values,
        std::size_t tuple_size,
        bool immutable)
    : DataAttribute(
        kTypeBool,
        immutable,
        new BoolStorage(values.begin(), values.end(), tuple_size)
    )
{
}

OMI_API_GLOBAL BoolAttribute::BoolAttribute(const Attribute& other)
    : DataAttribute(nullptr)
{
    assign(other);
}

OMI_API_GLOBAL BoolAttribute::BoolAttribute(const BoolAttribute& other)
    : DataAttribute(nullptr)
{
    assign(other);
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_GLOBAL BoolAttribute::~BoolAttribute()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL arc::str::UTF8String BoolAttribute::get_type_string()
{
    return "BoolAttribute";
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL BoolAttribute::DataType BoolAttribute::get_value() const
{
    // valid?
    check_state("get_value() used on an invalid attribute");

    // get the storage
    BoolStorage* storage = get_storage<BoolStorage>();

    // non-empty?
    if(storage->m_data.empty())
    {
        throw arc::ex::IndexOutOfBoundsError(
            "Failed to get single value from empty BoolAttribute"
        );
    }

    // return
    return storage->m_data.front();
}

OMI_API_GLOBAL
const BoolAttribute::ArrayType& BoolAttribute::get_values() const
{
    // valid?
    check_state("get_values() used on an invalid attribute");

    // get the storage and return
    return get_storage<BoolStorage>()->m_data;
}

OMI_API_GLOBAL void BoolAttribute::set_value(DataType value)
{
    // valid?
    check_state("set_value() used on an invalid attribute");

    prepare_modifcation();
    BoolStorage* storage = get_storage<BoolStorage>();
    storage->m_data = {value};
}

OMI_API_GLOBAL void BoolAttribute::set_values(const ArrayType& values)
{
    // valid?
    check_state("set_values() used on an invalid attribute");

    prepare_modifcation();
    get_storage<BoolStorage>()->m_data = values;
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL bool BoolAttribute::check_type(Type type) const
{
    return type == kTypeBool;
}

} // namespace omi
