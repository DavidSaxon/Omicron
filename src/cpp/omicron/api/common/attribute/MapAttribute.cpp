#include "omicron/api/common/attribute/MapAttribute.hpp"

#include <cassert>
#include <cstring>

#include <arcanecore/base/Exceptions.hpp>
#include <arcanecore/crypt/hash/Spooky.hpp>


namespace omi
{

//------------------------------------------------------------------------------
//                               STATIC ATTRIBUTES
//------------------------------------------------------------------------------

OMI_API_GLOBAL Attribute::Type MapAttribute::kTypeMap = 2;

//------------------------------------------------------------------------------
//                                    STORAGE
//------------------------------------------------------------------------------

//----------------------------C O N S T U C T O R S-----------------------------

OMI_API_GLOBAL MapAttribute::MapStorage::MapStorage()
{
}

//----------------------------D E S T R U C T O R S-----------------------------

OMI_API_GLOBAL MapAttribute::MapStorage::~MapStorage()
{
}

//---------------P U B L I C    M E M B E R    F U N C T I O N S----------------

OMI_API_GLOBAL bool MapAttribute::MapStorage::equals(const Storage* other) const
{
    // cast
    const MapStorage* casted = dynamic_cast<const MapStorage*>(other);
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
    for(auto entry : m_data)
    {
        auto f_entry = casted->m_data.find(entry.first);
        if(f_entry == casted->m_data.end())
        {
            return false;
        }
        if(entry.second != f_entry->second)
        {
            return false;
        }
    }
    return true;
}

OMI_API_GLOBAL bool MapAttribute::MapStorage::less_than(
        const Storage* other) const
{
    // cast
    const MapStorage* casted = dynamic_cast<const MapStorage*>(other);
    if(casted == nullptr)
    {
        return true;
    }
    // hmmm
    return m_data.size() < casted->m_data.size();
}

OMI_API_GLOBAL bool MapAttribute::MapStorage::is_data_pure_immutable() const
{
    for(auto entry : m_data)
    {
        if(!entry.second.is_pure_immutable())
        {
            return false;
        }
    }
    return true;
}

OMI_API_GLOBAL bool MapAttribute::MapStorage::is_data_pure_mutable() const
{
    // check sub attributes
    for(auto entry : m_data)
    {
        if(!entry.second.is_pure_mutable())
        {
            return false;
        }
    }
    return true;
}

OMI_API_GLOBAL
Attribute::Storage* MapAttribute::MapStorage::as_pure_immutable()
{
    // is the data already pure immutable?
    if(is_data_pure_immutable())
    {
        ++m_ref_count;
        return this;
    }

    // copy data and create new storage
    DataType pure_data;
    for(auto entry : m_data)
    {
        pure_data.insert(std::make_pair(
            entry.first,
            entry.second.as_pure_immutable()
        ));
    }
    return new MapStorage(pure_data.begin(), pure_data.end());

}

OMI_API_GLOBAL
Attribute::Storage* MapAttribute::MapStorage::as_pure_mutable()
{
    // copy data and create new storage
    DataType pure_data;
    for(auto entry : m_data)
    {
        pure_data.insert(std::make_pair(
            entry.first,
            entry.second.as_pure_mutable()
        ));
    }
    return new MapStorage(pure_data.begin(), pure_data.end());
}

OMI_API_GLOBAL Attribute::Hash MapAttribute::MapStorage::get_hash(
        arc::uint64 seed) const
{
    // iterate the sub hashes to make sure none have gone out of date
    bool out_of_date = false;
    for(auto child : m_data)
    {
        // get the child attribute's hash
        Hash attr_hash = child.second.get_hash();
        // out of date?
        auto f_sub_hash = m_sub_hashes.find(child.first);
        if(f_sub_hash == m_sub_hashes.end() ||
           f_sub_hash->second != attr_hash     )
        {
            m_sub_hashes[child.first] = attr_hash;
            out_of_date = true;
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
                    static_cast<const void*>(&sub_hash.second),
                    sizeof(sub_hash.second),
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

OMI_API_GLOBAL void MapAttribute::MapStorage::invalidate_hash()
{
    m_cached_hash.part1 = 0;
    m_cached_hash.part2 = 0;
    m_sub_hashes.clear();
}

OMI_API_GLOBAL Attribute::Storage* MapAttribute::MapStorage::copy_for_overwrite(
        bool soft)
{
    // soft overwrite - so copy everything
    if(soft)
    {
        return new MapStorage(m_data.begin(), m_data.end());
    }

    return new MapStorage();
}

OMI_API_GLOBAL void MapAttribute::MapStorage::string_repr(
        std::size_t indentation,
        arc::str::UTF8String& s) const
{
    // indentation?
    arc::str::UTF8String indent_str = " ";
    indent_str *= indentation;
    s << indent_str << "MapAttribute";
    // empty map?
    if(m_data.empty())
    {
        s << ": {}";
    }
    else
    {
        // next level of indentation
        std::size_t next_indentation = indentation + 4;
        // build the indentation string once
        for(auto entry : m_data)
        {
            s << "\n" << indent_str << "- " << entry.first << ":\n";
            entry.second.string_repr(s, next_indentation);
        }
    }
}

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL MapAttribute::MapAttribute(bool immutable)
    : Attribute(kTypeMap, immutable, new MapStorage())
{
}

OMI_API_GLOBAL MapAttribute::MapAttribute(const DataType& data, bool immutable)
    : Attribute(kTypeMap, immutable, new MapStorage(data.begin(), data.end()))
{
}

OMI_API_GLOBAL MapAttribute::MapAttribute(const Attribute& other)
    : Attribute(nullptr)
{
    assign(other);
}

OMI_API_GLOBAL MapAttribute::MapAttribute(const MapAttribute& other)
    : Attribute(nullptr)
{
    assign(other);
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

OMI_API_GLOBAL MapAttribute::~MapAttribute()
{
}

//------------------------------------------------------------------------------
//                                   OPERATORS
//------------------------------------------------------------------------------

OMI_API_GLOBAL const Attribute& MapAttribute::operator[](
            const arc::str::UTF8String& name) const
{
    return get(name);
}

OMI_API_GLOBAL Attribute& MapAttribute::operator[](
        const arc::str::UTF8String& name)
{
    return get(name);
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL std::size_t MapAttribute::get_size() const
{
    // valid?
    check_state("get_size() used on an invalid attribute");

    return get_storage<MapStorage>()->m_data.size();
}

OMI_API_GLOBAL const MapAttribute::DataType& MapAttribute::get_values() const
{
    // valid?
    check_state("get_values() used on an invalid attribute");

    return get_storage<MapStorage>()->m_data;
}

OMI_API_GLOBAL
std::vector<arc::str::UTF8String> MapAttribute::get_names() const
{
    // valid?
    check_state("get_names() used on an invalid attribute");

    std::vector<arc::str::UTF8String> names;
    names.reserve(get_size());
    for(auto entry : get_values())
    {
        names.push_back(entry.first);
    }
    return names;
}

OMI_API_GLOBAL std::vector<Attribute> MapAttribute::get_attributes() const
{
    // valid?
    check_state("get_attributes() used on an invalid attribute");

    std::vector<Attribute> attributes;
    attributes.reserve(get_size());
    for(auto entry : get_values())
    {
        attributes.push_back(entry.second);
    }
    return attributes;
}

OMI_API_GLOBAL bool MapAttribute::has(const arc::str::UTF8String& name) const
{
    // valid?
    check_state("has() used on an invalid attribute");

    const DataType& data = get_values();
    // nested?
    std::size_t delimiter = name.find_first(".");
    if(delimiter == arc::str::npos)
    {
        return data.find(name) != data.end();
    }
    else
    {
        auto f_data = data.find(name.substring(0, delimiter));
        if(f_data == data.end())
        {
            return false;
        }
        omi::MapAttribute sub = f_data->second;
        if(!sub.is_valid())
        {
            return false;
        }
        return sub.has(name.substring(delimiter + 1, name.get_length()));
    }
}

OMI_API_GLOBAL const Attribute& MapAttribute::get(
        const arc::str::UTF8String& name) const
{
    // valid?
    check_state("get() used on an invalid attribute");

    const DataType& data = get_values();
    // nested?
    std::size_t delimiter = name.find_first(".");
    if(delimiter == arc::str::npos)
    {
        auto f_data = data.find(name);
        if(f_data == data.end())
        {
            throw arc::ex::KeyError(
                "No entry in MapAttribute under name \"" + name + "\""
            );
        }
        return f_data->second;
    }
    else
    {
        auto f_data = data.find(name.substring(0, delimiter));
        if(f_data == data.end())
        {
            throw arc::ex::KeyError(
                "No entry in MapAttribute under name \"" +
                name.substring(0, delimiter) + "\""
            );
        }
        omi::MapAttribute sub = f_data->second;
        if(!sub.is_valid())
        {
            throw arc::ex::KeyError(
                "No nested MapAttribute in MapAttribute under name \"" +
                name.substring(0, delimiter) + "\""
            );

        }
        return sub.get(name.substring(delimiter + 1, name.get_length()));
    }
}

OMI_API_GLOBAL Attribute& MapAttribute::get(const arc::str::UTF8String& name)
{
    // valid?
    check_state("get() used on an invalid attribute");

    DataType& data = get_storage<MapStorage>()->m_data;
    // nested?
    std::size_t delimiter = name.find_first(".");
    if(delimiter == arc::str::npos)
    {
        auto f_data = data.find(name);
        if(f_data == data.end())
        {
            throw arc::ex::KeyError(
                "No entry in MapAttribute under name \"" + name + "\""
            );
        }
        return f_data->second;
    }
    else
    {
        auto f_data = data.find(name.substring(0, delimiter));
        if(f_data == data.end())
        {
            throw arc::ex::KeyError(
                "No entry in MapAttribute under name \"" +
                name.substring(0, delimiter) + "\""
            );
        }
        omi::MapAttribute sub = f_data->second;
        if(!sub.is_valid())
        {
            throw arc::ex::KeyError(
                "No nested MapAttribute in MapAttribute under name \"" +
                name.substring(0, delimiter) + "\""
            );

        }
        return sub.get(name.substring(delimiter + 1, name.get_length()));
    }
}

OMI_API_GLOBAL void MapAttribute::insert(
        const arc::str::UTF8String& name,
        const Attribute& attrribute)
{
    // valid?
    check_state("insert() used on an invalid attribute");
    prepare_modifcation();

    DataType& data = get_storage<MapStorage>()->m_data;
    // nested?
    std::size_t delimiter = name.find_first(".");
    if(delimiter == arc::str::npos)
    {
        get_storage<MapStorage>()->m_data[name] = attrribute;
    }
    else
    {
        auto f_data = data.find(name.substring(0, delimiter));
        if(f_data == data.end())
        {
            throw arc::ex::KeyError(
                "No entry in MapAttribute under name \"" +
                name.substring(0, delimiter) + "\""
            );
        }
        omi::MapAttribute sub = f_data->second;
        if(!sub.is_valid())
        {
            throw arc::ex::KeyError(
                "No nested MapAttribute in MapAttribute under name \"" +
                name.substring(0, delimiter) + "\""
            );

        }
        sub.insert(
            name.substring(delimiter + 1, name.get_length()),
            attrribute
        );
    }
}

OMI_API_GLOBAL void MapAttribute::erase(const arc::str::UTF8String& name)
{
    // valid?
    check_state("erase() used on an invalid attribute");
    prepare_modifcation(true);

    DataType& data = get_storage<MapStorage>()->m_data;
    // nested?
    std::size_t delimiter = name.find_first(".");
    if(delimiter == arc::str::npos)
    {
        auto f_data = data.find(name);
        if(f_data == data.end())
        {
            throw arc::ex::KeyError(
                "No entry in MapAttribute under name \"" + name + "\""
            );
        }
        data.erase(f_data);
    }
    else
    {
        auto f_data = data.find(name.substring(0, delimiter));
        if(f_data == data.end())
        {
            throw arc::ex::KeyError(
                "No entry in MapAttribute under name \"" +
                name.substring(0, delimiter) + "\""
            );
        }
        omi::MapAttribute sub = f_data->second;
        if(!sub.is_valid())
        {
            throw arc::ex::KeyError(
                "No nested MapAttribute in MapAttribute under name \"" +
                name.substring(0, delimiter) + "\""
            );

        }
        sub.erase(name.substring(delimiter + 1, name.get_length()));
    }
}

OMI_API_GLOBAL void MapAttribute::set_values(const DataType& data)
{
    // valid?
    check_state("set_values() used on an invalid attribute");
    prepare_modifcation();
    get_storage<MapStorage>()->m_data = DataType(data.begin(), data.end());
}

OMI_API_GLOBAL void MapAttribute::clear()
{
    // valid?
    check_state("clear() used on an invalid attribute");
    prepare_modifcation();
    get_storage<MapStorage>()->m_data.clear();
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL bool MapAttribute::check_type(Type type) const
{
    return type == kTypeMap;
}

} // namespace omi
