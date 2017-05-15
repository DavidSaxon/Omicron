#include "omicron/config/ConfigGlobals.hpp"

namespace omi
{
namespace config
{
namespace global
{

OMI_CONFIG_GLOBAL_API const arc::io::sys::Path root_dir(
    {arc::str::UTF8String("config"), arc::str::UTF8String("omicron")});

} // namespace global
} // namespace config
} // namespace omi
