#include "omicron_runtime/meta/MetaGlobals.hpp"

namespace omi_
{
namespace meta
{
namespace global
{

// TOOD: could source meta directory from libomicron
const arc::io::sys::Path runtime_dir(
    {arc::str::UTF8String("meta"), arc::str::UTF8String("omicron_runtime")}
);

} // namespace global
} // namespace meta
} // namespace omi_
