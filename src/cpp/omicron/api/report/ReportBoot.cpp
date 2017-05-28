#include "omicron/api/report/ReportBoot.hpp"

#include "omicron/api/report/Logging.hpp"


namespace omi
{
namespace report
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL void startup_routine()
{
    omi::report::logging_startup_routine();
}

OMI_API_GLOBAL void shutdown_routine()
{
}

} // namespace report
} // namespace omi
