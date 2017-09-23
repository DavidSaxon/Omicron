#include "omicron/api/report/stats/StatsDatabase.hpp"

#include <unordered_map>

#include <arcanecore/base/Exceptions.hpp>
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

    /*!
     * \brief The config document for the ResourceRegistry.
     */
    arc::config::DocumentPtr m_config_data;

    /*!
     * \brief Mapping from entry names to the associated attributes.
     */
    std::unordered_map<arc::str::UTF8String, omi::DataAttribute> m_entries;

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

    void define_entry(const arc::str::UTF8String& name, omi::DataAttribute attr)
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

    void execute_query(StatsQuery& query) const
    {
        // TODO: get the names of all of the entries
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
        omi::DataAttribute attr)
{
    m_impl->define_entry(name, attr);
}

OMI_API_GLOBAL const omi::DataAttribute& StatsDatabase::get_entry(
        const arc::str::UTF8String& name) const
{
    return m_impl->get_entry(name);
}

OMI_API_GLOBAL void StatsDatabase::execute_query(StatsQuery& query) const
{
    m_impl->execute_query(query);
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
