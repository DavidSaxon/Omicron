#include "omicron/api/common/attribute/Int32Attribute.hpp"

#include <cassert>
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
    DataAttribute::kTypeDataBits & (typeid(arc::int32).hash_code() >> 8);

//------------------------------------------------------------------------------
//                                   DEFINITION
//------------------------------------------------------------------------------

//---------------------------C O N S T R U C T O R S----------------------------

// OMI_API_GLOBAL Int32Attribute::Int32Definition::Int32Definition(
//         std::size_t tuple_size,
//         bool immutable)
//     : DataAttribute::DataDefinition(kTypeInt32, immutable)
// {
//     set_storage(new Int32Storage(tuple_size));
// }

// //-----------------------------D E S T R U C T O R------------------------------

// OMI_API_GLOBAL Int32Attribute::Int32Definition::~Int32Definition()
// {
// }

// //---------------P U B L I C    M E M B E R    F U N C T I O N S----------------

// OMI_API_GLOBAL std::size_t Int32Attribute::Int32Definition::get_size() const
// {
//     return get_storage<Int32Storage>()->m_data.size();
// }

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL Int32Attribute::Int32Attribute(bool immutable)
    : DataAttribute(kTypeInt32, immutable, new Int32Storage(0))
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
        const std::vector<DataType>& values,
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
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL Int32Attribute::DataType Int32Attribute::get_value(
        bool throw_on_error,
        DataType default_value) const
{
    // valid?
    if(throw_on_error)
    {
        check_state();
    }
    else if(!is_valid())
    {
        return default_value;
    }

    // get the storage
    Int32Storage* storage = get_storage<Int32Storage>();

    // non-empty?
    if(storage->m_data.empty())
    {
        if(throw_on_error)
        {
            throw arc::ex::IndexOutOfBoundsError(
                "Failed to get single value from empty Int32Attribute"
            );
        }
        else
        {
            return default_value;
        }
    }

    // return
    return storage->m_data.front();
}

OMI_API_GLOBAL const Int32Attribute::ArrayType& Int32Attribute::get_values(
        bool throw_on_error)
{
    // valid?
    if(throw_on_error)
    {
        check_state();
    }
    else if(!is_valid())
    {
        return g_empty;
    }

    // get the storage and return
    return get_storage<Int32Storage>()->m_data;
}

OMI_API_GLOBAL void Int32Attribute::set_value(DataType value)
{
    check_state();
    prepare_modifcation();
    Int32Storage* storage = get_storage<Int32Storage>();
    storage->m_data = {value};
    storage->m_tuple_size = 0;
}

OMI_API_GLOBAL void Int32Attribute::set_values(const ArrayType& values)
{
    check_state();
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

OMI_API_GLOBAL void Int32Attribute::check_state() const
{
    if(!is_valid())
    {
        throw arc::ex::StateError("Invalid Int32Attribute");
    }
}

} // namespace omi
