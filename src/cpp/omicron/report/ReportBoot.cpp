#include "omicron/report/ReportBoot.hpp"

#include "omicron/report/Logging.hpp"

namespace omi
{
namespace report
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

OMI_REPORT_GLOBAL_API void startup_routine()
{
    omi::report::logging_startup_routine();
}

OMI_REPORT_GLOBAL_API void shutdown_routine()
{
}

} // namespace report
} // namespace omi
