#include "omicron/api/asset/AssetGlobals.hpp"

#include "omicron/api/config/ConfigGlobals.hpp"


namespace omi
{
namespace asset
{
namespace global
{

arc::log::Input* logger = nullptr;

const arc::io::sys::Path config_root_dir(
    arc::io::sys::Path(omi::config::global::root_dir).join("asset"));

} // namespace global
} // namespace asset
} // namespace omi
