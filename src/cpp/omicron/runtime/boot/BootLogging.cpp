#include "omicron/runtime/boot/BootLogging.hpp"

#include <metaengine/Document.hpp>

#include <omicron/report/Logging.hpp>

#include "omicron/runtime/RuntimeGlobals.hpp"

namespace omi
{
namespace runtime
{
namespace boot
{

//------------------------------------------------------------------------------
//                                   PROTOTYPES
//------------------------------------------------------------------------------

/*!
 * \brief Reports when a MetaEngine Document has failed to load, and a fallback
 *        protocol must be executed.
 */
static void load_fallback_reporter(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String& message);

/*!
 * \brief Reports when retrieving a value from a MetaEngine Document has failed,
 *        and a fallback protocol must be executed.
 */
static void get_fallback_reporter(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String& message);

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

bool startup_logging_subroutine()
{
    // create the logging profile
    arclog::Profile profile("OMICRON-RUNTIME");
    // vend the input from the shared handler
    omi::runtime::global::logger = omi::report::log_handler.vend_input(profile);

    // connect fallback reporters for MetaEngine
    metaengine::Document::set_load_fallback_reporter(load_fallback_reporter);
    metaengine::Document::set_get_fallback_reporter(get_fallback_reporter);

    return true;
}

bool shutdown_logging_subroutine()
{
    return true;
}

static void load_fallback_reporter(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String& message)
{
    omi::runtime::global::logger->error
        << "MetaEngine error loading data associated with file \""
        << file_path << "\": " << message << std::endl;
}

static void get_fallback_reporter(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String& message)
{
    omi::runtime::global::logger->error
        << "MetaEngine error accessing data in \"" << file_path << "\": "
        << message << std::endl;
}

} // namespace boot
} // namespace runtime
} // namespace omi
