#include "omicron/api/res/ResourceId.hpp"

#include <arcanecore/crypt/hash/Spooky.hpp>

namespace omi
{
namespace res
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

OMI_API_GLOBAL ResourceId get_id(const arc::str::UTF8String& resource_path)
{
    // use 64-bit spooky hash
    return arc::crypt::hash::spooky_64(
        resource_path.get_raw(),
        resource_path.get_byte_length() - 1
    );
}

OMI_API_GLOBAL ResourceId get_id(const arc::io::sys::Path& resource_path)
{
    return get_id(resource_path.to_unix());
}

} // namespace res
} // namespace omi
