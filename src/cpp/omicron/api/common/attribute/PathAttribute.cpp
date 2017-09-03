#include "omicron/api/common/attribute/PathAttribute.hpp"

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
static PathAttribute::ArrayType g_empty;

} // namespace anonymous

//------------------------------------------------------------------------------
//                               STATIC ATTRIBUTES
//------------------------------------------------------------------------------

OMI_API_GLOBAL Attribute::Type PathAttribute::kTypePath =
    DataAttribute::kTypeDataBits |
    (typeid(PathAttribute::DataType).hash_code() >> 8);

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL PathAttribute::PathAttribute()
    : DataAttribute(kTypePath, true, new PathStorage(0))
{
}

OMI_API_GLOBAL PathAttribute::PathAttribute(DataType value, bool immutable)
    : DataAttribute(
        kTypePath,
        immutable,
        new PathStorage(&value, (&value) + 1, 0)
    )
{
}

OMI_API_GLOBAL PathAttribute::PathAttribute(
        const ArrayType& values,
        std::size_t tuple_size,
        bool immutable)
    : DataAttribute(
        kTypePath,
        immutable,
        new PathStorage(values.begin(), values.end(), tuple_size)
    )
{
}

OMI_API_GLOBAL PathAttribute::PathAttribute(const Attribute& other)
    : DataAttribute(nullptr)
{
    assign(other);
}

OMI_API_GLOBAL PathAttribute::PathAttribute(const PathAttribute& other)
    : DataAttribute(nullptr)
{
    assign(other);
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_GLOBAL PathAttribute::~PathAttribute()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL arc::str::UTF8String PathAttribute::get_type_string()
{
    return "PathAttribute";
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL PathAttribute::DataType PathAttribute::get_value() const
{
    // valid?
    check_state("get_value() used on an invalid attribute");

    // get the storage
    PathStorage* storage = get_storage<PathStorage>();

    // non-empty?
    if(storage->m_data.empty())
    {
        throw arc::ex::IndexOutOfBoundsError(
            "Failed to get single value from empty PathAttribute"
        );
    }

    // return
    return storage->m_data.front();
}

OMI_API_GLOBAL
const PathAttribute::ArrayType& PathAttribute::get_values() const
{
    // valid?
    check_state("get_values() used on an invalid attribute");

    // get the storage and return
    return get_storage<PathStorage>()->m_data;
}

OMI_API_GLOBAL void PathAttribute::set_value(DataType value)
{
    // valid?
    check_state("set_value() used on an invalid attribute");

    prepare_modifcation();
    PathStorage* storage = get_storage<PathStorage>();
    storage->m_data = {value};
}

OMI_API_GLOBAL void PathAttribute::set_values(const ArrayType& values)
{
    // valid?
    check_state("set_values() used on an invalid attribute");

    prepare_modifcation();
    get_storage<PathStorage>()->m_data = values;
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL bool PathAttribute::check_type(Type type) const
{
    return type == kTypePath;
}

} // namespace omi
