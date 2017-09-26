#include "omicron/api/report/SystemMonitor.hpp"

#include <arcanecore/config/Document.hpp>
#include <arcanecore/config/visitors/Shorthand.hpp>

#include "omicron/api/report/stats/StatsDatabase.hpp"


namespace omi
{
namespace report
{

//------------------------------------------------------------------------------
//                                 IMPLEMENTATION
//------------------------------------------------------------------------------

class SystemMonitor::SystemMonitorImpl
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
private:

    //-------------------P R I V A T E    A T T R I B U T E S-------------------

    // The config document for the system monitor.
    arc::config::DocumentPtr m_config_data;

    omi::Int64Attribute m_current_rss;
    arc::str::UTF8String m_current_rss_key;

    omi::Int64Attribute m_peak_rss;
    arc::str::UTF8String m_peak_rss_key;


    //------------P R I V A T E    M E M B E R    F U N C T I O N S-------------

    // defines the statistics in the StatsDatabase
    void define_stats()
    {
        omi::report::StatsDatabase::instance()->define_entry(
            m_current_rss_key,
            m_current_rss,
            "The last sampled size of the engine's Resident Set Size (RSS) "
            "i.e. RAM usage."
        );
        omi::report::StatsDatabase::instance()->define_entry(
            m_peak_rss_key,
            m_peak_rss,
            "The peak sampled value that the engine's Resident Set Size "
            "reached."
        );
    }

public:

    //--------------------------C O N S T R U C T O R---------------------------

    SystemMonitorImpl()
        : m_current_rss    (0, false)
        , m_current_rss_key("System.Memory.RSS")
        , m_peak_rss       (0, false)
        , m_peak_rss_key   ("System.Memory.Peak RSS")
    {
    }

    //---------------------------D E S T R U C T O R----------------------------

    ~SystemMonitorImpl()
    {
    }

    //-------------P U B L I C    M E M B E R    F U N C T I O N S--------------

    bool startup_routine()
    {
        define_stats();

        // TODO: load config

        return true;
    }

    bool shutdown_routine()
    {
        return true;
    }

    void update(bool force)
    {
        // TODO:
    }

    omi::Int64Attribute get_current_rss() const
    {
        return m_current_rss.as_immutable();
    }

    const arc::str::UTF8String& get_current_rss_key() const
    {
        return m_current_rss_key;
    }

    omi::Int64Attribute get_peak_rss() const
    {
        return m_peak_rss.as_immutable();
    }

    const arc::str::UTF8String& get_peak_rss_key() const
    {
        return m_peak_rss_key;
    }
};

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL SystemMonitor* SystemMonitor::instance()
{
    static SystemMonitor inst;
    return &inst;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL bool SystemMonitor::startup_routine()
{
    return m_impl->startup_routine();
}

OMI_API_GLOBAL bool SystemMonitor::shutdown_routine()
{
    return m_impl->shutdown_routine();
}

OMI_API_GLOBAL void SystemMonitor::update(bool force)
{
    m_impl->update(force);
}

OMI_API_GLOBAL omi::Int64Attribute SystemMonitor::get_current_rss() const
{
    return m_impl->get_current_rss();
}

OMI_API_GLOBAL
const arc::str::UTF8String& SystemMonitor::get_current_rss_key() const
{
    return m_impl->get_current_rss_key();
}

OMI_API_GLOBAL omi::Int64Attribute SystemMonitor::get_peak_rss() const
{
    return m_impl->get_peak_rss();
}

OMI_API_GLOBAL
const arc::str::UTF8String& SystemMonitor::get_peak_rss_key() const
{
    return m_impl->get_peak_rss_key();
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTOR
//------------------------------------------------------------------------------

SystemMonitor::SystemMonitor()
    : m_impl(new SystemMonitorImpl())
{
}

//------------------------------------------------------------------------------
//                               PRIVATE DESTRUCTOR
//------------------------------------------------------------------------------

SystemMonitor::~SystemMonitor()
{
    delete m_impl;
}

} // namespace report
} // namespace omi
