#include "omicron/api/res/ResourceGlobals.hpp"

#include "omicron/api/config/ConfigGlobals.hpp"


namespace omi
{
namespace res
{
namespace global
{

arc::log::Input* logger = nullptr;

const arc::io::sys::Path config_root_dir(
    arc::io::sys::Path(omi::config::global::root_dir).join("res"));

} // namespace global
} // namespace res
} // namespace omi
