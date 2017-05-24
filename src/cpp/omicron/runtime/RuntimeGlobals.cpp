#include "omicron/runtime/RuntimeGlobals.hpp"

#include <omicron/api/config/ConfigGlobals.hpp>


namespace omi
{
namespace runtime
{
namespace global
{

arc::log::Input* logger = nullptr;

const arc::io::sys::Path config_root_dir(
    arc::io::sys::Path(omi::config::global::root_dir).join("runtime"));

} // namespace global
} // namespace runtime
} // namespace omi
