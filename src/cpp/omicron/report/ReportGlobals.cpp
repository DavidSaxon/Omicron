#include "omicron/report/ReportGlobals.hpp"

#include <omicron/meta/MetaGlobals.hpp>

namespace omi
{
namespace report
{
namespace global
{

const arc::io::sys::Path meta_root_dir(
    arc::io::sys::Path(omi::meta::global::root_dir).join("report"));

const arc::io::sys::Path meta_logging_dir(
    arc::io::sys::Path(meta_root_dir).join("logging"));

} // namespace global
} // namespace report
} // namespace omi
