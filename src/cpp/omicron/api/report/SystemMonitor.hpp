/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_API_REPORT_SYSTEMMONITOR_HPP_
#define OMICRON_API_REPORT_SYSTEMMONITOR_HPP_

#include <arcanecore/base/lang/Restrictors.hpp>
#include <arcanecore/base/str/UTF8String.hpp>

#include "omicron/api/API.hpp"
#include "omicron/api/common/Attributes.hpp"


namespace omi
{
namespace report
{

class SystemMonitor
    : private arc::lang::Noncopyable
    , private arc::lang::Nonmovable
    , private arc::lang::Noncomparable
{
public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the SystemMonitor.
     */
    OMI_API_EXPORT static SystemMonitor* instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    //-----------------------------ENGINE INTERNALS-----------------------------
    // hide from doxygen
    #ifndef IN_DOXYGEN

    /*!
     * \brief Initialises the SystemMonitor.
     */
    OMI_API_EXPORT bool startup_routine();

    /*!
     * \brief Shutdowns the SystemMonitor.
     */
    OMI_API_EXPORT bool shutdown_routine();

    #endif
    // IN_DOXYGEN
    //--------------------------------------------------------------------------

    /*!
     * \brief Is called once per frame to stochastically update statistics or
     *        can be manually called with the force parameter set to ```true```
     *        to force all system statics to be updated.
     */
    OMI_API_EXPORT void update(bool force = false);

    /*!
     * \brief Returns the name of this machine's operating system.
     */
    OMI_API_EXPORT const arc::str::UTF8String& get_os_name() const;

    /*!
     * \brief Returns the distribution name/version of this machine's operating
     *        system.
     */
    OMI_API_EXPORT const arc::str::UTF8String& get_os_distro() const;

    /*!
     * \brief Returns the model of this machine's CPU.
     */
    OMI_API_EXPORT const arc::str::UTF8String& get_cpu_model() const;

    /*!
     * \brief Returns the number of physical cores this machine's CPU has.
     */
    OMI_API_EXPORT std::size_t get_cpu_physial_cores() const;

    /*!
     * \brief Returns the number of logical processing units this machine's CPU
     *        has.
     */
    OMI_API_EXPORT std::size_t get_cpu_logical_processors() const;

    /*!
     * \brief Returns the clock rate (in MHz) of this machine's CPU.
     */
    OMI_API_EXPORT float get_cpu_clock_rate() const;

    /*!
     * \brief Returns the id of the logical processor the primary thread of the
     *        engine is running on.
     */
    OMI_API_EXPORT std::size_t get_primary_processor() const;

    /*!
     * \brief Returns the total amount of RAM on this system (in megabytes).
     */
    OMI_API_EXPORT double get_total_ram() const;

    /*!
     * \brief Returns the amount of free RAM on this system (in megabytes).
     */
    OMI_API_EXPORT double get_free_ram() const;

    /*!
     * \brief Returns the total amount of virtual memory on this system
     *        (in megabytes).
     */
    OMI_API_EXPORT double get_total_virtual_memory() const;

    /*!
     * \brief Returns the amount of free virtual memory on this system (in
     *        megabytes).
     */
    OMI_API_EXPORT double get_free_virtual_memory() const;

    /*!
     * \brief Returns the last sampled size of the engine's Resident Set Size
     *        (in megabytes).
     */
    OMI_API_EXPORT double get_current_rss() const;

    /*!
     * \brief Returns the peak sampled value that engine's Residents Set Size
     *        reached (in megabytes).
     */
    OMI_API_EXPORT double get_peak_rss() const;

private:

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTOR
    //--------------------------------------------------------------------------

    SystemMonitor();

    //--------------------------------------------------------------------------
    //                             PRIVATE DESTRUCTOR
    //--------------------------------------------------------------------------

    ~SystemMonitor();

    //--------------------------------------------------------------------------
    //                            COMPILATION FIREWALL
    //--------------------------------------------------------------------------

    class SystemMonitorImpl;
    SystemMonitorImpl* m_impl;
};

} // namespace report
} // namespace omi

#endif
