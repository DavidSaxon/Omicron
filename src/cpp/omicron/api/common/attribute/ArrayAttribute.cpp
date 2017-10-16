#include "omicron/api/common/attribute/ArrayAttribute.hpp"

#include <cassert>
#include <cstring>

#include <arcanecore/base/Exceptions.hpp>
#include <arcanecore/crypt/hash/Spooky.hpp>

namespace omi
{

//------------------------------------------------------------------------------
//                               STATIC ATTRIBUTES
//------------------------------------------------------------------------------

OMI_API_EXPORT Attribute::Type ArrayAttribute::kTypeArray = 1;

//------------------------------------------------------------------------------
//                                    STORAGE
//------------------------------------------------------------------------------

//----------------------------C O N S T U C T O R S-----------------------------

OMI_API_EXPORT ArrayAttribute::ArrayStorage::ArrayStorage()
{
}

//----------------------------D E S T R U C T O R S-----------------------------

OMI_API_EXPORT ArrayAttribute::ArrayStorage::~ArrayStorage()
{
}

//---------------P U B L I C    M E M B E R    F U N C T I O N S----------------

OMI_API_EXPORT bool ArrayAttribute::ArrayStorage::equals(
        const Storage* other) const
{
    // cast
    const ArrayStorage* casted = dynamic_cast<const ArrayStorage*>(other);
    if(casted == nullptr)
    {
        return false;
    }

    // check data size
    if(m_data.size() != casted->m_data.size())
    {
        return false;
    }

    // check entries
    for(std::size_t i = 0; i < m_data.size(); ++i)
    {
        if(m_data[i] != casted->m_data[i])
        {
            return false;
        }
    }
    return true;
}

OMI_API_EXPORT bool ArrayAttribute::ArrayStorage::less_than(
        const Storage* other) const
{
    // cast
    const ArrayStorage* casted = dynamic_cast<const ArrayStorage*>(other);
    if(casted == nullptr)
    {
        return true;
    }
    return m_data < casted->m_data;
}

OMI_API_EXPORT bool ArrayAttribute::ArrayStorage::is_data_pure_immutable() const
{
    for(auto entry : m_data)
    {
        if(!entry.is_pure_immutable())
        {
            return false;
        }
    }
    return true;
}

OMI_API_EXPORT bool ArrayAttribute::ArrayStorage::is_data_pure_mutable() const
{
    // check sub attributes
    for(auto entry : m_data)
    {
        if(!entry.is_pure_mutable())
        {
            return false;
        }
    }
    return true;
}

OMI_API_EXPORT
Attribute::Storage* ArrayAttribute::ArrayStorage::as_pure_immutable()
{
    // is the data already pure immutable?
    if(is_data_pure_immutable())
    {
        ++m_ref_count;
        return this;
    }

    // copy data and create new storage
    DataType pure_data;
    pure_data.reserve(m_data.size());
    for(auto entry : m_data)
    {
        pure_data.push_back(entry.as_pure_immutable());
    }
    return new ArrayStorage(pure_data.begin(), pure_data.end());

}

OMI_API_EXPORT
Attribute::Storage* ArrayAttribute::ArrayStorage::as_pure_mutable()
{
    // copy data and create new storage
    DataType pure_data;
    pure_data.reserve(m_data.size());
    for(auto entry : m_data)
    {
        pure_data.push_back(entry.as_pure_mutable());
    }
    return new ArrayStorage(pure_data.begin(), pure_data.end());
}

OMI_API_EXPORT Attribute::Hash ArrayAttribute::ArrayStorage::get_hash(
        arc::uint64 seed) const
{
    // iterate the sub hashes to make sure none have gone out of date
    bool out_of_date = false;
    if(m_data.size() != m_sub_hashes.size())
    {
        out_of_date = true;
        // clear and rebuild
        m_sub_hashes.clear();
        m_sub_hashes.reserve(m_data.size());
        for(std::size_t i = 0; i < m_data.size(); ++i)
        {
            m_sub_hashes.push_back(m_data[i].get_hash());
        }
    }
    else
    {
        for(std::size_t i = 0; i < m_sub_hashes.size(); ++i)
        {
            Hash attr_hash = m_data[i].get_hash();
            if(m_sub_hashes[i] != attr_hash)
            {
                m_sub_hashes[i] = attr_hash;
                out_of_date = true;
            }
        }
    }
    if(out_of_date)
    {
        m_cached_hash.part1 = 0;
        m_cached_hash.part2 = 0;
    }

    // hash needs recomputing?
    if(m_cached_hash.part1 == 0 && m_cached_hash.part2 == 0)
    {
        // start with the seed
        m_cached_hash.part1 = seed;
        m_cached_hash.part2 = seed;

        // is there actually anything to hash?
        if(!m_data.empty())
        {
            assert(m_sub_hashes.size() == m_data.size());

            // compute the hash
            for(auto sub_hash : m_sub_hashes)
            {
                arc::crypt::hash::spooky_128(
                    static_cast<const void*>(&sub_hash),
                    sizeof(sub_hash),
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

OMI_API_EXPORT void ArrayAttribute::ArrayStorage::invalidate_hash()
{
    m_cached_hash.part1 = 0;
    m_cached_hash.part2 = 0;
    m_sub_hashes.clear();
}

OMI_API_EXPORT
Attribute::Storage* ArrayAttribute::ArrayStorage::copy_for_overwrite(bool soft)
{
    // soft overwrite - so copy everything
    if(soft)
    {
        return new ArrayStorage(m_data.begin(), m_data.end());
    }

    return new ArrayStorage();
}

OMI_API_EXPORT void ArrayAttribute::ArrayStorage::string_repr(
        std::size_t indentation,
        arc::str::UTF8String& s) const
{
    // indentation?
    if(indentation > 0)
    {
        s << (arc::str::UTF8String(" ") * indentation);
    }
    s << "[";
    // values
    for(std::size_t i = 0; i < m_data.size(); ++i)
    {
        s << m_data[i];
        if(i < m_data.size() - 1)
        {
            s << ", ";
        }
    }
    s << "]";
}

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_EXPORT ArrayAttribute::ArrayAttribute(bool immutable)
    : Attribute(kTypeArray, immutable, new ArrayStorage())
{
}

OMI_API_EXPORT ArrayAttribute::ArrayAttribute(
        const DataType& data,
        bool immutable)
    : Attribute(
        kTypeArray,
        immutable,
        new ArrayStorage(data.begin(), data.end())
    )
{
}

OMI_API_EXPORT ArrayAttribute::ArrayAttribute(const Attribute& other)
    : Attribute(nullptr)
{
    assign(other);
}

OMI_API_EXPORT ArrayAttribute::ArrayAttribute(const ArrayAttribute& other)
    : Attribute(nullptr)
{
    assign(other);
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_EXPORT ArrayAttribute::~ArrayAttribute()
{
}

//------------------------------------------------------------------------------
//                                   OPERATORS
//------------------------------------------------------------------------------

OMI_API_EXPORT const Attribute& ArrayAttribute::operator[](
        std::size_t index) const
{
    return get(index);
}

OMI_API_EXPORT Attribute& ArrayAttribute::operator[](std::size_t index)
{
    return get(index);
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT std::size_t ArrayAttribute::get_size() const
{
    // valid?
    check_state("get_size() used on an invalid attribute");

    return get_storage<ArrayStorage>()->m_data.size();
}

OMI_API_EXPORT bool ArrayAttribute::is_empty() const
{
    // valid?
    check_state("get_size() used on an invalid attribute");

    return get_storage<ArrayStorage>()->m_data.empty();
}

OMI_API_EXPORT
    const ArrayAttribute::DataType& ArrayAttribute::get_values() const
{
    // valid?
    check_state("get_values() used on an invalid attribute");

    return get_storage<ArrayStorage>()->m_data;
}

OMI_API_EXPORT const Attribute& ArrayAttribute::get(std::size_t index) const
{
    // valid?
    check_state("get() used on an invalid attribute");

    // within range
    if(index >= get_size())
    {
        arc::str::UTF8String error_message;
        error_message
            << "Index: " << index << " greater than or equal to array size "
            << "of: " << get_size();
        throw arc::ex::IndexOutOfBoundsError(error_message);
    }

    return get_storage<ArrayStorage>()->m_data[index];
}

OMI_API_EXPORT Attribute& ArrayAttribute::get(std::size_t index)
{
    // valid?
    check_state("get() used on an invalid attribute");

    // within range
    if(index >= get_size())
    {
        arc::str::UTF8String error_message;
        error_message
            << "Index: " << index << " greater than or equal to array size "
            << "of: " << get_size();
        throw arc::ex::IndexOutOfBoundsError(error_message);
    }

    return get_storage<ArrayStorage>()->m_data[index];
}

OMI_API_EXPORT const Attribute& ArrayAttribute::front() const
{
    // valid?
    check_state("front() used on an invalid attribute");

    // within range
    if(get_size() == 0)
    {
        throw arc::ex::IndexOutOfBoundsError(
            "Cannot retrieve front of empty array"
        );
    }

    return get_storage<ArrayStorage>()->m_data.front();
}

OMI_API_EXPORT Attribute& ArrayAttribute::front()
{
    // valid?
    check_state("front() used on an invalid attribute");

    // within range
    if(get_size() == 0)
    {
        throw arc::ex::IndexOutOfBoundsError(
            "Cannot retrieve front of empty array"
        );
    }

    return get_storage<ArrayStorage>()->m_data.front();
}

OMI_API_EXPORT const Attribute& ArrayAttribute::back() const
{
    // valid?
    check_state("back() used on an invalid attribute");

    // within range
    if(get_size() == 0)
    {
        throw arc::ex::IndexOutOfBoundsError(
            "Cannot retrieve back of empty array"
        );
    }

    return get_storage<ArrayStorage>()->m_data.back();
}

OMI_API_EXPORT Attribute& ArrayAttribute::back()
{
    // valid?
    check_state("back() used on an invalid attribute");

    // within range
    if(get_size() == 0)
    {
        throw arc::ex::IndexOutOfBoundsError(
            "Cannot retrieve back of empty array"
        );
    }

    return get_storage<ArrayStorage>()->m_data.back();
}

OMI_API_EXPORT void ArrayAttribute::set(
        std::size_t index,
        const Attribute& attribute)
{
    // valid?
    check_state("set() used on an invalid attribute");
    prepare_modifcation(true);

    // within range
    if(index >= get_size())
    {
        arc::str::UTF8String error_message;
        error_message
            << "Index: " << index << " greater than or equal to array size "
            << "of: " << get_size();
        throw arc::ex::IndexOutOfBoundsError(error_message);
    }

    get_storage<ArrayStorage>()->m_data[index] = attribute;
}

OMI_API_EXPORT void ArrayAttribute::push_back(const Attribute& attribute)
{
    // valid?
    check_state("push_back() used on an invalid attribute");
    prepare_modifcation(true);

    get_storage<ArrayStorage>()->m_data.push_back(attribute);
}

OMI_API_EXPORT void ArrayAttribute::insert(
        std::size_t index,
        const Attribute& attribute)
{
    // valid?
    check_state("insert() used on an invalid attribute");
    prepare_modifcation(true);

    // within range
    if(index > get_size())
    {
        arc::str::UTF8String error_message;
        error_message
            << "Index: " << index << " greater than array size of: "
            << get_size();
        throw arc::ex::IndexOutOfBoundsError(error_message);
    }

    get_storage<ArrayStorage>()->m_data.insert(
        get_storage<ArrayStorage>()->m_data.begin() + index,
        attribute
    );
}

OMI_API_EXPORT void ArrayAttribute::erase(std::size_t index)
{
    // valid?
    check_state("erase() used on an invalid attribute");
    prepare_modifcation(true);

    // within range
    if(index >= get_size())
    {
        arc::str::UTF8String error_message;
        error_message
            << "Index: " << index << " greater than or equal to array size "
            << "of: " << get_size();
        throw arc::ex::IndexOutOfBoundsError(error_message);
    }

    get_storage<ArrayStorage>()->m_data.erase(
        get_storage<ArrayStorage>()->m_data.begin() + index
    );
}

OMI_API_EXPORT void ArrayAttribute::set_values(const DataType& data)
{
    // valid?
    check_state("set_values() used on an invalid attribute");
    prepare_modifcation();
    get_storage<ArrayStorage>()->m_data = DataType(data.begin(), data.end());
}

OMI_API_EXPORT void ArrayAttribute::clear()
{
    // valid?
    check_state("set_values() used on an invalid attribute");
    prepare_modifcation();
    get_storage<ArrayStorage>()->m_data.clear();
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_EXPORT bool ArrayAttribute::check_type(Type type) const
{
    return type == kTypeArray;
}

} // namespace omi
