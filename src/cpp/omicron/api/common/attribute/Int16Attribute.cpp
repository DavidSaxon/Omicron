#include "omicron/api/common/attribute/Int16Attribute.hpp"

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
static Int16Attribute::ArrayType g_empty;

} // namespace anonymous

//------------------------------------------------------------------------------
//                               STATIC ATTRIBUTES
//------------------------------------------------------------------------------

OMI_API_GLOBAL Attribute::Type Int16Attribute::kTypeInt16 =
    DataAttribute::kTypeDataBits |
    (typeid(Int16Attribute::DataType).hash_code() >> 8);

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL Int16Attribute::Int16Attribute(bool immutable)
    : DataAttribute(kTypeInt16, immutable, new Int16Storage(0))
{
}

OMI_API_GLOBAL Int16Attribute::Int16Attribute(DataType value, bool immutable)
    : DataAttribute(
        kTypeInt16,
        immutable,
        new Int16Storage(&value, (&value) + 1, 0)
    )
{
}

OMI_API_GLOBAL Int16Attribute::Int16Attribute(
        const ArrayType& values,
        std::size_t tuple_size,
        bool immutable)
    : DataAttribute(
        kTypeInt16,
        immutable,
        new Int16Storage(values.begin(), values.end(), tuple_size)
    )
{
}

OMI_API_GLOBAL Int16Attribute::Int16Attribute(const Attribute& other)
    : DataAttribute(nullptr)
{
    assign(other);
}

OMI_API_GLOBAL Int16Attribute::Int16Attribute(const Int16Attribute& other)
    : DataAttribute(nullptr)
{
    assign(other);
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_GLOBAL Int16Attribute::~Int16Attribute()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL arc::str::UTF8String Int16Attribute::get_type_string()
{
    return "Int16Attribute";
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL Int16Attribute::DataType Int16Attribute::get_value() const
{
    // valid?
    check_state("get_value() used on an invalid attribute");

    // get the storage
    Int16Storage* storage = get_storage<Int16Storage>();

    // non-empty?
    if(storage->m_data.empty())
    {
        throw arc::ex::IndexOutOfBoundsError(
            "Failed to get single value from empty Int16Attribute"
        );
    }

    // return
    return storage->m_data.front();
}

OMI_API_GLOBAL
const Int16Attribute::ArrayType& Int16Attribute::get_values() const
{
    // valid?
    check_state("get_values() used on an invalid attribute");

    // get the storage and return
    return get_storage<Int16Storage>()->m_data;
}

OMI_API_GLOBAL void Int16Attribute::set_value(DataType value)
{
    // valid?
    check_state("set_value() used on an invalid attribute");

    prepare_modifcation();
    Int16Storage* storage = get_storage<Int16Storage>();
    storage->m_data = {value};
}

OMI_API_GLOBAL void Int16Attribute::set_values(const ArrayType& values)
{
    // valid?
    check_state("set_values() used on an invalid attribute");

    prepare_modifcation();
    get_storage<Int16Storage>()->m_data = values;
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL bool Int16Attribute::check_type(Type type) const
{
    return type == kTypeInt16;
}

} // namespace omi
