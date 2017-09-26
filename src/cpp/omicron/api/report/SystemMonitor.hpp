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
    OMI_API_GLOBAL static SystemMonitor* instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    //-----------------------------ENGINE INTERNALS-----------------------------
    // hide from doxygen
    #ifndef IN_DOXYGEN

    /*!
     * \brief Initialises the SystemMonitor.
     */
    OMI_API_GLOBAL bool startup_routine();

    /*!
     * \brief Shutdowns the SystemMonitor.
     */
    OMI_API_GLOBAL bool shutdown_routine();

    #endif
    // IN_DOXYGEN
    //--------------------------------------------------------------------------

    /*!
     * \brief Is called once per frame to stochastically update statistics or
     *        can be manually called with the force parameter set to ```true```
     *        to force all system statics to be updated.
     */
    OMI_API_GLOBAL void update(bool force = false);

    /*!
     * \brief Returns an immutable reference to the statistics attribute which
     *        stores current resident set size.
     */
    OMI_API_GLOBAL omi::Int64Attribute get_current_rss() const;

    /*!
     * \brief Returns the StatsDatabase key for current resident set size.
     */
    OMI_API_GLOBAL const arc::str::UTF8String& get_current_rss_key() const;

    /*!
     * \brief Return an immutable reference to the statistics attribute which
     *        stores peak resident set size.
     */
    OMI_API_GLOBAL omi::Int64Attribute get_peak_rss() const;

    /*!
     * \brief Returns the StatsDatabase key for peak resident set size.
     */
    OMI_API_GLOBAL const arc::str::UTF8String& get_peak_rss_key() const;

    // TODO: more stats

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
