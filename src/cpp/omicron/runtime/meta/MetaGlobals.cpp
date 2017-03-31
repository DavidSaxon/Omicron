#include "omicron/runtime/meta/MetaGlobals.hpp"

namespace omi
{
namespace runtime
{
namespace meta
{
namespace global
{

// TOOD: could source meta directory from libomicron
const arc::io::sys::Path runtime_dir({
    arc::str::UTF8String("meta"),
    arc::str::UTF8String("omicron"),
    arc::str::UTF8String("runtime")
});

} // namespace global
} // namespace meta
} // namespace runtime
} // namespace omi
