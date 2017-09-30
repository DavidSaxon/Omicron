#include "omicron/api/report/SystemMonitor.hpp"

#include <arcanecore/base/data/DataConstants.hpp>
#include <arcanecore/io/os/SysInfo.hpp>
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

    omi::StringAttribute m_cpu_model;
    omi::Int64Attribute m_cpu_physical_cores;
    omi::Int64Attribute m_cpu_logical_processors;
    omi::FloatAttribute m_cpu_clock_rate;
    omi::DoubleAttribute m_current_rss;
    omi::DoubleAttribute m_peak_rss;

    //------------P R I V A T E    M E M B E R    F U N C T I O N S-------------

    // defines the statistics in the StatsDatabase
    void define_stats()
    {
        omi::report::StatsDatabase::instance()->define_entry(
            "System.CPU.Model",
            m_cpu_model,
            "The model name of this machine's CPU."
        );
        omi::report::StatsDatabase::instance()->define_entry(
            "System.CPU.Physical Cores",
            m_cpu_physical_cores,
            "The number of physical cores this machine's CPU has."
        );
        omi::report::StatsDatabase::instance()->define_entry(
            "System.CPU.Logical Processing Units",
            m_cpu_logical_processors,
            "The number of logical processing units this machine's CPU has."
        );
        omi::report::StatsDatabase::instance()->define_entry(
            "System.CPU.Clock Rate (MHz)",
            m_cpu_clock_rate,
            "The estimated clock rate of this machine's CPU."
        );
        omi::report::StatsDatabase::instance()->define_entry(
            "System.Memory.RSS (mb)",
            m_current_rss,
            "The last sampled size of the engine's Resident Set Size (RSS) "
            "i.e. RAM usage."
        );
        omi::report::StatsDatabase::instance()->define_entry(
            "System.Memory.Peak RSS (mb)",
            m_peak_rss,
            "The peak sampled value that the engine's Resident Set Size "
            "reached."
        );
    }

public:

    //--------------------------C O N S T R U C T O R---------------------------

    SystemMonitorImpl()
        : m_cpu_model             ("", false)
        , m_cpu_physical_cores    (0, false)
        , m_cpu_logical_processors(0, false)
        , m_cpu_clock_rate        (0.0F, false)
        , m_current_rss           (0, false)
        , m_peak_rss              (0, false)
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

        std::cout << "startup_routine called!" << std::endl;

        // get the one system stats CPU stats
        m_cpu_model.set_at(0, arc::io::os::get_cpu_model());
        m_cpu_physical_cores.set_at(0, arc::io::os::get_cpu_physical_cores());
        m_cpu_logical_processors.set_at(
            0,
            arc::io::os::get_cpu_logical_processors()
        );
        m_cpu_clock_rate.set_at(0, arc::io::os::get_cpu_clock_rate());

        return true;
    }

    bool shutdown_routine()
    {
        return true;
    }

    void update(bool force)
    {
        // TODO: stochastic sampling
        m_current_rss.set_at(
            0,
            static_cast<double>(arc::io::os::get_rss()) /
            static_cast<double>(arc::data::BYTE_TO_MEGABYTE)
        );
        m_peak_rss.set_at(
            0,
            static_cast<double>(arc::io::os::get_peak_rss()) /
            static_cast<double>(arc::data::BYTE_TO_MEGABYTE)
        );
    }

    const arc::str::UTF8String& get_cpu_model() const
    {
        return m_cpu_model.at(0);
    }

    std::size_t get_cpu_physial_cores() const
    {
        return static_cast<std::size_t>(m_cpu_physical_cores.at(0));
    }

    std::size_t get_cpu_logical_processors() const
    {
        return static_cast<std::size_t>(m_cpu_logical_processors.at(0));
    }

    float get_cpu_clock_rate() const
    {
        return m_cpu_clock_rate.at(0);
    }

    double get_current_rss() const
    {
        return m_current_rss.at(0);
    }

    double get_peak_rss() const
    {
        return m_peak_rss.at(0);
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

OMI_API_GLOBAL const arc::str::UTF8String& SystemMonitor::get_cpu_model() const
{
    return m_impl->get_cpu_model();
}

OMI_API_GLOBAL std::size_t SystemMonitor::get_cpu_physial_cores() const
{
    return m_impl->get_cpu_physial_cores();
}

OMI_API_GLOBAL std::size_t SystemMonitor::get_cpu_logical_processors() const
{
    return m_impl->get_cpu_logical_processors();
}

OMI_API_GLOBAL float SystemMonitor::get_cpu_clock_rate() const
{
    return m_impl->get_cpu_clock_rate();
}

OMI_API_GLOBAL double SystemMonitor::get_current_rss() const
{
    return m_impl->get_current_rss();
}

OMI_API_GLOBAL double SystemMonitor::get_peak_rss() const
{
    return m_impl->get_peak_rss();
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
