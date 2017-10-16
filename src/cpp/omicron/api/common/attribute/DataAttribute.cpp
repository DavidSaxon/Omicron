#include "omicron/api/common/attribute/DataAttribute.hpp"

#include <arcanecore/base/Exceptions.hpp>
#include <arcanecore/crypt/hash/Spooky.hpp>


namespace omi
{

//------------------------------------------------------------------------------
//                               STATIC ATTRIBUTES
//------------------------------------------------------------------------------

Attribute::Type DataAttribute::kTypeDataBits = 4;

//------------------------------------------------------------------------------
//                                    STORAGE
//------------------------------------------------------------------------------

//----------------------------C O N S T R U C T O R-----------------------------

OMI_API_EXPORT DataAttribute::DataStorage::DataStorage(std::size_t tuple_size)
    : Attribute::Storage()
    , m_tuple_size      (tuple_size)
{
}

//-----------------------------D E S T R U C T O R------------------------------

OMI_API_EXPORT DataAttribute::DataStorage::~DataStorage()
{
}

//---------------P U B L I C    M E M B E R    F  U N C T I O N S---------------

OMI_API_EXPORT bool DataAttribute::DataStorage::is_data_pure_immutable() const
{
    // mutability not relevant
    return true;
}

OMI_API_EXPORT bool DataAttribute::DataStorage::is_data_pure_mutable() const
{
    // mutability not relevant
    return true;
}

OMI_API_EXPORT Attribute::Storage*
DataAttribute::DataStorage::as_pure_immutable()
{
    ++m_ref_count;
    return this;
}

OMI_API_EXPORT
Attribute::Storage* DataAttribute::DataStorage::as_pure_mutable()
{
    ++m_ref_count;
    return this;
}

OMI_API_EXPORT void DataAttribute::DataStorage::compute_hash(
        const void* data,
        std::size_t length,
        arc::uint64 seed,
        Hash& out_hash) const
{
    arc::crypt::hash::spooky_128(
        data,
        length,
        out_hash.part1,
        out_hash.part2,
        seed,
        m_tuple_size
    );
}

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_EXPORT DataAttribute::DataAttribute()
    : Attribute(kTypeNull, true, nullptr)
{
}

OMI_API_EXPORT DataAttribute::DataAttribute(const Attribute& other)
    : Attribute(nullptr)
{
    assign(other);
}

OMI_API_EXPORT DataAttribute::DataAttribute(const DataAttribute& other)
    : Attribute(nullptr)
{
    assign(other);
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT DataAttribute::~DataAttribute()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT std::size_t DataAttribute::get_size() const
{
    check_state("get_size() used on an invalid attribute");
    return get_storage<DataStorage>()->get_size();
}

OMI_API_EXPORT std::size_t DataAttribute::get_tuple_size() const
{
    check_state("get_tuple_size() used on an invalid attribute");
    return get_storage<DataStorage>()->m_tuple_size;
}

OMI_API_EXPORT void DataAttribute::set_tuple_size(std::size_t tuple_size)
{
    check_state("set_tuple_size() used on an invalid attribute");
    // soft modification
    prepare_modifcation(true);
    get_storage<DataStorage>()->m_tuple_size = tuple_size;
}

//------------------------------------------------------------------------------
//                             PROTECTED CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_EXPORT DataAttribute::DataAttribute(Definition* def)
    : Attribute(def)
{
}

OMI_API_EXPORT DataAttribute::DataAttribute(
        Type type,
        bool immutable,
        Storage* storage)
    : Attribute(type, immutable, storage)
{
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT bool DataAttribute::check_type(Type type) const
{
    return type & kTypeDataBits;
}

} // namespace omi
