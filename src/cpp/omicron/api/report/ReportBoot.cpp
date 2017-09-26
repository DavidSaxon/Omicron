#include "omicron/api/report/ReportBoot.hpp"

#include "omicron/api/report/Logging.hpp"
#include "omicron/api/report/SystemMonitor.hpp"


namespace omi
{
namespace report
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL bool startup_routine()
{
    omi::report::logging_startup_routine();
    if(!omi::report::SystemMonitor::instance()->startup_routine())
    {
        return false;
    }

    return true;
}

OMI_API_GLOBAL bool shutdown_routine()
{
    if(!omi::report::SystemMonitor::instance()->shutdown_routine())
    {
        return false;
    }
    return true;
}

} // namespace report
} // namespace omi
