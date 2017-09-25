#include "omicron/api/common/attribute/PathAttribute.hpp"

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
static PathAttribute::ArrayType g_empty;

} // namespace anonymous

//------------------------------------------------------------------------------
//                               STATIC ATTRIBUTES
//------------------------------------------------------------------------------

OMI_API_GLOBAL Attribute::Type PathAttribute::kTypePath =
    DataAttribute::kTypeDataBits |
    (typeid(PathAttribute::DataType).hash_code() >> 8);

//------------------------------------------------------------------------------
//                                    STORAGE
//------------------------------------------------------------------------------

//---------------------------C O N S T R U C T O R S----------------------------

OMI_API_GLOBAL PathAttribute::PathStorage::PathStorage(
        std::size_t tuple_size)
    : TypedDataStorage<DataType>(tuple_size)
{
}

//-----------------------------D E S T R U C T O R------------------------------

OMI_API_GLOBAL PathAttribute::PathStorage::~PathStorage()
{
}

//---------------P U B L I C    M E M B E R    F U N C T I O N S----------------

OMI_API_GLOBAL Attribute::Hash PathAttribute::PathStorage::get_hash(
        arc::uint64 seed) const
{
    // hash need recomputing?
    if(m_cached_hash.part1 == 0 && m_cached_hash.part2 == 0)
    {
        // start with seed and tuple size
        m_cached_hash.part1 = seed;
        m_cached_hash.part2 = m_tuple_size;

        // hash each string in each path
        for(const DataType& d : m_data)
        {
            // for(const arc::str::UTF8String& s : d)
            for(std::size_t i = 0; i < d.get_length(); ++i)
            {
                arc::crypt::hash::spooky_128(
                    static_cast<const void*>(d[i].get_raw()),
                    d[i].get_byte_length(),
                    m_cached_hash.part1,
                    m_cached_hash.part2,
                    m_cached_hash.part1,
                    m_cached_hash.part2
                );
            }
        }
    }
    return m_cached_hash;
}

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

OMI_API_GLOBAL const PathAttribute::DataType& PathAttribute::at(
        std::size_t index) const
{
    // valid?
    check_state("at() used on an invalid attribute");

    // get the storage
    PathStorage* storage = get_storage<PathStorage>();

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

OMI_API_GLOBAL void PathAttribute::set_at(
        std::size_t index,
        DataType value)
{
    // valid?
    check_state("at() used on an invalid attribute");

    // get the storage
    PathStorage* storage = get_storage<PathStorage>();

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

OMI_API_GLOBAL bool PathAttribute::check_type(Type type) const
{
    return type == kTypePath;
}

} // namespace omi
