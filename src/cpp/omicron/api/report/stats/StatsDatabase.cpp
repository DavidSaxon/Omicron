#include "omicron/api/report/stats/StatsDatabase.hpp"

#include <unordered_map>

#include <arcanecore/base/Exceptions.hpp>
#include <arcanecore/base/str/FnMatch.hpp>
#include <arcanecore/base/str/UTF8String.hpp>
#include <arcanecore/config/Document.hpp>
#include <arcanecore/config/visitors/Shorthand.hpp>

#include "omicron/api/report/stats/StatsQuery.hpp"


namespace omi
{
namespace report
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class StatsDatabase::StatsDatabaseImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // The config document for the ResourceRegistry.
    arc::config::DocumentPtr m_config_data;

    // Mapping from entry names to the associated attributes.
    std::unordered_map<arc::str::UTF8String, omi::DataAttribute> m_entries;

    // Mapping from entry names to the associated descriptions
    std::unordered_map<arc::str::UTF8String, arc::str::UTF8String>
        m_descriptions;

    // empty string - for returning non-existent descriptions
    const arc::str::UTF8String m_empty_string;

public:

    //--------------------------C O N S T R U C T O R---------------------------

    StatsDatabaseImpl()
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~StatsDatabaseImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    void define_entry(
            const arc::str::UTF8String& name,
            omi::DataAttribute attr,
            const arc::str::UTF8String& description)
    {
        // ensure this is a new entry
        auto f_entry = m_entries.find(name);
        if(f_entry != m_entries.end())
        {
            throw arc::ex::KeyError(
                "StatsDatabase entry already exists with the name \"" + name +
                "\""
            );
        }

        // ensure the attribute is valid
        if(!attr.is_valid())
        {
            throw arc::ex::ValueError(
                "StatsDatabase supplied with invalid attribute"
            );
        }

        // ensure the attribute is mutable
        if(attr.is_immutable())
        {
            throw arc::ex::ValueError(
                "StatsDatabase supplied with immutable attribute"
            );
        }

        // add the entry
        m_entries.insert(std::make_pair(name, attr));
        // add to descriptions?
        if(!description.is_empty())
        {
            m_descriptions.insert(std::make_pair(name, description));
        }
    }

    const omi::DataAttribute& get_entry(const arc::str::UTF8String& name) const
    {
        // is there an entry with the name?
        auto f_entry = m_entries.find(name);
        if(f_entry == m_entries.end())
        {
            throw arc::ex::KeyError(
                "No StatsDatabase entry with name: \"" + name + "\""
            );
        }

        return f_entry->second;
    }

    const arc::str::UTF8String& get_description(
            const arc::str::UTF8String& name) const
    {
        auto f_description = m_descriptions.find(name);
        if(f_description != m_descriptions.end())
        {
            return f_description->second;
        }
        return m_empty_string;
    }

    std::vector<arc::str::UTF8String> get_names() const
    {
        std::vector<arc::str::UTF8String> ret;
        ret.reserve(m_entries.size());
        for(auto entry : m_entries)
        {
            ret.push_back(entry.first);
        }
        return ret;
    }

    void execute_query(StatsQuery& query, StatsQuery::Result& result) const
    {
        for(auto entry : m_entries)
        {
            // does the name match one of the entries in the query?
            for(const arc::str::UTF8String& pattern : query.get_patterns())
            {
                if(arc::str::fnmatch(pattern, entry.first))
                {
                    result.insert(entry);
                    break;
                }
            }
        }
    }
};

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL StatsDatabase* StatsDatabase::instance()
{
    static StatsDatabase inst;
    return &inst;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL void StatsDatabase::define_entry(
        const arc::str::UTF8String& name,
        omi::DataAttribute attr,
        const arc::str::UTF8String& description)
{
    m_impl->define_entry(name, attr, description);
}

OMI_API_GLOBAL const omi::DataAttribute& StatsDatabase::get_entry(
        const arc::str::UTF8String& name) const
{
    return m_impl->get_entry(name);
}

OMI_API_GLOBAL
std::vector<arc::str::UTF8String> StatsDatabase::get_names() const
{
    return m_impl->get_names();
}

OMI_API_GLOBAL void StatsDatabase::execute_query(StatsQuery& query) const
{
    m_impl->execute_query(query, query.m_result);
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

StatsDatabase::StatsDatabase()
    : m_impl(new StatsDatabaseImpl())
{
}

//------------------------------------------------------------------------------
//                               PRIVATE DESTRUCTOR
//------------------------------------------------------------------------------

StatsDatabase::~StatsDatabase()
{
    delete m_impl;
}

} // namespace report
} // namespace omi
