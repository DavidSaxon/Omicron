#include "omicron/api/common/attribute/Int32Attribute.hpp"

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
static Int32Attribute::ArrayType g_empty;

} // namespace anonymous

//------------------------------------------------------------------------------
//                               STATIC ATTRIBUTES
//------------------------------------------------------------------------------

OMI_API_GLOBAL Attribute::Type Int32Attribute::kTypeInt32 =
    DataAttribute::kTypeDataBits |
    (typeid(Int32Attribute::DataType).hash_code() >> 8);

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL Int32Attribute::Int32Attribute()
    : DataAttribute(kTypeInt32, true, new Int32Storage(0))
{
}

OMI_API_GLOBAL Int32Attribute::Int32Attribute(DataType value, bool immutable)
    : DataAttribute(
        kTypeInt32,
        immutable,
        new Int32Storage(&value, (&value) + 1, 0)
    )
{
}

OMI_API_GLOBAL Int32Attribute::Int32Attribute(
        const ArrayType& values,
        std::size_t tuple_size,
        bool immutable)
    : DataAttribute(
        kTypeInt32,
        immutable,
        new Int32Storage(values.begin(), values.end(), tuple_size)
    )
{
}

OMI_API_GLOBAL Int32Attribute::Int32Attribute(const Attribute& other)
    : DataAttribute(nullptr)
{
    assign(other);
}

OMI_API_GLOBAL Int32Attribute::Int32Attribute(const Int32Attribute& other)
    : DataAttribute(nullptr)
{
    assign(other);
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_GLOBAL Int32Attribute::~Int32Attribute()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL arc::str::UTF8String Int32Attribute::get_type_string()
{
    return "Int32Attribute";
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL Int32Attribute::DataType Int32Attribute::get_value() const
{
    // valid?
    check_state("get_value() used on an invalid attribute");

    // get the storage
    Int32Storage* storage = get_storage<Int32Storage>();

    // non-empty?
    if(storage->m_data.empty())
    {
        throw arc::ex::IndexOutOfBoundsError(
            "Failed to get single value from empty Int32Attribute"
        );
    }

    // return
    return storage->m_data.front();
}

OMI_API_GLOBAL
const Int32Attribute::ArrayType& Int32Attribute::get_values() const
{
    // valid?
    check_state("get_values() used on an invalid attribute");

    // get the storage and return
    return get_storage<Int32Storage>()->m_data;
}

OMI_API_GLOBAL void Int32Attribute::set_value(DataType value)
{
    // valid?
    check_state("set_value() used on an invalid attribute");

    prepare_modifcation();
    Int32Storage* storage = get_storage<Int32Storage>();
    storage->m_data = {value};
}

OMI_API_GLOBAL void Int32Attribute::set_values(const ArrayType& values)
{
    // valid?
    check_state("set_values() used on an invalid attribute");

    prepare_modifcation();
    get_storage<Int32Storage>()->m_data = values;
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL bool Int32Attribute::check_type(Type type) const
{
    return type == kTypeInt32;
}

} // namespace omi
