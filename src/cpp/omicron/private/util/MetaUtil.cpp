#include "omicron/private/util/MetaUtil.hpp"

namespace omi
{
namespace util
{
namespace meta
{

//------------------------------------------------------------------------------
//                                   CONSTANTS
//------------------------------------------------------------------------------

const arc::io::sys::Path META_DIR({arc::str::UTF8String("meta")});
const arc::io::sys::Path META_CORE_DIR(
    {arc::str::UTF8String("meta"), arc::str::UTF8String("core")});

} // namespace meta
} // namespace util
} // namespace omi
