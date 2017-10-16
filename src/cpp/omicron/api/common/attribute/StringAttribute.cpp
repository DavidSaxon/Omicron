#include "omicron/api/common/attribute/StringAttribute.hpp"

#include <typeinfo>

#include <arcanecore/base/Exceptions.hpp>
#include <arcanecore/crypt/hash/Spooky.hpp>


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

OMI_API_EXPORT Attribute::Type StringAttribute::kTypeString =
    DataAttribute::kTypeDataBits |
    (typeid(StringAttribute::DataType).hash_code() >> 8);

//------------------------------------------------------------------------------
//                                    STORAGE
//------------------------------------------------------------------------------

//---------------------------C O N S T R U C T O R S----------------------------

OMI_API_EXPORT StringAttribute::StringStorage::StringStorage(
        std::size_t tuple_size)
    : TypedDataStorage<DataType>(tuple_size)
{
}

//-----------------------------D E S T R U C T O R------------------------------

OMI_API_EXPORT StringAttribute::StringStorage::~StringStorage()
{
}

//---------------P U B L I C    M E M B E R    F U N C T I O N S----------------

OMI_API_EXPORT Attribute::Hash StringAttribute::StringStorage::get_hash(
        arc::uint64 seed) const
{
    // hash need recomputing?
    if(m_cached_hash.part1 == 0 && m_cached_hash.part2 == 0)
    {
        // start with seed and tuple size
        m_cached_hash.part1 = seed;
        m_cached_hash.part2 = m_tuple_size;

        // hash each string
        for(const DataType& d : m_data)
        {
            arc::crypt::hash::spooky_128(
                static_cast<const void*>(d.get_raw()),
                d.get_byte_length(),
                m_cached_hash.part1,
                m_cached_hash.part2,
                m_cached_hash.part1,
                m_cached_hash.part2
            );
        }
    }
    return m_cached_hash;
}

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_EXPORT StringAttribute::StringAttribute()
    : DataAttribute(kTypeString, true, new StringStorage(0))
{
}

OMI_API_EXPORT StringAttribute::StringAttribute(DataType value, bool immutable)
    : DataAttribute(
        kTypeString,
        immutable,
        new StringStorage(&value, (&value) + 1, 0)
    )
{
}

OMI_API_EXPORT StringAttribute::StringAttribute(
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

OMI_API_EXPORT StringAttribute::StringAttribute(const Attribute& other)
    : DataAttribute(nullptr)
{
    assign(other);
}

OMI_API_EXPORT StringAttribute::StringAttribute(const StringAttribute& other)
    : DataAttribute(nullptr)
{
    assign(other);
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT StringAttribute::~StringAttribute()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT arc::str::UTF8String StringAttribute::get_type_string()
{
    return "StringAttribute";
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT StringAttribute::DataType StringAttribute::get_value() const
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

OMI_API_EXPORT
const StringAttribute::ArrayType& StringAttribute::get_values() const
{
    // valid?
    check_state("get_values() used on an invalid attribute");

    // get the storage and return
    return get_storage<StringStorage>()->m_data;
}

OMI_API_EXPORT const StringAttribute::DataType& StringAttribute::at(
        std::size_t index) const
{
    // valid?
    check_state("at() used on an invalid attribute");

    // get the storage
    StringStorage* storage = get_storage<StringStorage>();

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

OMI_API_EXPORT void StringAttribute::set_value(DataType value)
{
    // valid?
    check_state("set_value() used on an invalid attribute");

    prepare_modifcation();
    StringStorage* storage = get_storage<StringStorage>();
    storage->m_data = {value};
}

OMI_API_EXPORT void StringAttribute::set_values(const ArrayType& values)
{
    // valid?
    check_state("set_values() used on an invalid attribute");

    prepare_modifcation();
    get_storage<StringStorage>()->m_data = values;
}

OMI_API_EXPORT void StringAttribute::set_at(
        std::size_t index,
        DataType value)
{
    // valid?
    check_state("at() used on an invalid attribute");

    // get the storage
    StringStorage* storage = get_storage<StringStorage>();

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

OMI_API_EXPORT bool StringAttribute::check_type(Type type) const
{
    return type == kTypeString;
}

} // namespace omi
