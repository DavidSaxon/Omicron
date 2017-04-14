#include "omicron/runtime/RuntimeGlobals.hpp"

#include <omicron/meta/MetaGlobals.hpp>

namespace omi
{
namespace runtime
{
namespace global
{

arclog::Input* logger = nullptr;

const arc::io::sys::Path meta_root_dir(
    arc::io::sys::Path(omi::meta::global::root_dir).join("runtime"));

} // namespace global
} // namespace runtime
} // namespace omi
