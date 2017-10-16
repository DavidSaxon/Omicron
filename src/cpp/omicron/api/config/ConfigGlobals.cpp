#include "omicron/api/config/ConfigGlobals.hpp"


namespace omi
{
namespace config
{
namespace global
{

OMI_API_EXPORT const arc::io::sys::Path root_dir(
    {arc::str::UTF8String("config"), arc::str::UTF8String("omicron")});

} // namespace global
} // namespace config
} // namespace omi
