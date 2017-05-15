#include "omicron/report/ReportGlobals.hpp"

#include <omicron/config/ConfigGlobals.hpp>

namespace omi
{
namespace report
{
namespace global
{

const arc::io::sys::Path config_root_dir(
    arc::io::sys::Path(omi::config::global::root_dir).join("report"));

const arc::io::sys::Path config_logging_dir(
    arc::io::sys::Path(config_root_dir).join("logging"));

} // namespace global
} // namespace report
} // namespace omi
