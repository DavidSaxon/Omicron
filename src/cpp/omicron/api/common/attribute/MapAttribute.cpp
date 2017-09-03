#include "omicron/api/common/attribute/MapAttribute.hpp"

#include <arcanecore/base/Exceptions.hpp>


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
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL std::size_t MapAttribute::get_size() const
{
    // valid?
    check_state("get_size() used on an invalid attribute");

    return get_storage<MapStorage>()->m_data.size();
}

OMI_API_GLOBAL const MapAttribute::DataType& MapAttribute::get_data() const
{
    // valid?
    check_state("get_data() used on an invalid attribute");

    return get_storage<MapStorage>()->m_data;
}

OMI_API_GLOBAL
std::vector<arc::str::UTF8String> MapAttribute::get_names() const
{
    // valid?
    check_state("get_names() used on an invalid attribute");

    std::vector<arc::str::UTF8String> names;
    names.reserve(get_size());
    for(auto entry : get_data())
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
    for(auto entry : get_data())
    {
        attributes.push_back(entry.second);
    }
    return attributes;
}

OMI_API_GLOBAL bool MapAttribute::has(const arc::str::UTF8String& name) const
{
    // valid?
    check_state("has() used on an invalid attribute");

    const DataType& data = get_data();
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

    const DataType& data = get_data();
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

OMI_API_GLOBAL void MapAttribute::set_data(const DataType& data)
{
    // valid?
    check_state("set_data() used on an invalid attribute");
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
