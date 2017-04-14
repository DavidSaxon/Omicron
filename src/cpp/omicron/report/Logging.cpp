#include "omicron/report/Logging.hpp"

#include <chrono>
#include <ctime>
#include <iostream>

#include <arcanecore/base/Preproc.hpp>

#include <arcanelog/outputs/FileOutput.hpp>
#include <arcanelog/outputs/StdOutput.hpp>

#include <json/json.h>

#include <metaengine/Variant.hpp>
#include <metaengine/visitors/Shorthand.hpp>

#include <omicron/meta/MetaInline.hpp>

#include "omicron/report/ReportGlobals.hpp"

// allows us to use std::localtime, without warning it's unsafe, maybe this is
// bad idea, but I can't imagine it being too detrimental since it's only being
// used to determine log file names
#ifdef ARC_OS_WINDOWS
    #pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
#endif

namespace omi
{
namespace report
{

//------------------------------------------------------------------------------
//                                    GLOBALS
//------------------------------------------------------------------------------

arclog::LogHandler log_handler;

/*!
 * \brief The MetaEngine Variant for logging configuration.
 */
static metaengine::VariantPtr g_metadata;

/*!
 * \brief The logging output to std::cout and std::cerr.
 */
arclog::StdOutput* std_output;
/*!
 * \brief The logging output for writing to the file system.
 */
arclog::FileOutput* file_output;

//------------------------------------------------------------------------------
//                                    CLASSES
//------------------------------------------------------------------------------

/*!
 * \brief MetaEngine Visitor object used to retrieve arclog::Verbosity values
 *                   from a metaengine::Document.
 *
 * Verbosity values must be expressed in the document as one of the following
 * strings:
 *
 * - critical
 * - error
 * - warning
 * - notice
 * - info
 * - debug
 */
class ArcLogVerbosityV : public metaengine::Visitor<arclog::Verbosity>
{
public:

    /*!
     * \brief Provides an existing static instance of this object.
     */
    static ArcLogVerbosityV& instance();

    // override
    virtual bool retrieve(
            const Json::Value* data,
            const arc::str::UTF8String& key,
            metaengine::Document* requester,
            arc::str::UTF8String& error_message);
};

//------------------------------------------------------------------------------
//                                   PROTOTYPES
//------------------------------------------------------------------------------

/*!
 * \brief Temporary reporter function which will print MetaEngine loading errors
 *        to std::cerr
 */
static void std_load_reporter(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String& message);

/*!
 * \brief Temporary reporter function which will print MetaEngine retrieval
 *        errors to std::cerr
 */
static void std_get_reporter(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String& message);


/*!
 * \brief Initialises the StdOutput logging writer.
 */
static void init_std_output();

/*!
 * \brief Initialises the FileOutput logging writer.
 */
static void init_file_output();


//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

void logging_startup_routine()
{
    // logging MetaEngine data needs to be loaded before all other MetaEngine
    // data since we want to initialise logging as early as possible. Since
    // logging has not yet been initialised we will use some temporary fallback
    // reporter functions that write to std::cerr
    metaengine::Document::set_load_fallback_reporter(std_load_reporter);
    metaengine::Document::set_get_fallback_reporter(std_get_reporter);

    // // build the path to the base logging document
    arc::io::sys::Path meta_path(omi::report::global::meta_logging_dir);
    meta_path << "logging.json";

    // built-in memory data
    static const arc::str::UTF8String meta_compiled(
        OMICRON_META_INLINE_REPORT_LOGGING
    );

    // construct the variant
    g_metadata.reset(new metaengine::Variant(
        meta_path,
        &meta_compiled
    ));
    // use unix variant?
    #ifdef ARC_OS_UNIX
        g_metadata->set_variant("unix");
    #endif

    // setup outputs
    init_std_output();
    init_file_output();
}

static void std_load_reporter(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String& message)
{
    std::cerr << "MetaEngine error loading data associated with file \""
              << file_path << "\": " << message << std::endl;
}

static void std_get_reporter(
        const arc::io::sys::Path& file_path,
        const arc::str::UTF8String& message)
{
    std::cerr << "MetaEngine error accessing data in \"" << file_path
              << "\": " << message << std::endl;
}

static void init_std_output()
{
    // stdoutput
    std_output = new arclog::StdOutput();
    // enabled?
    bool enabled = *g_metadata->get("outputs.StdOutput.enabled", ME_BOOLV);
    std_output->set_enabled(enabled);
    // verbosity
    std_output->set_verbosity_level(*g_metadata->get(
        "outputs.StdOutput.verbosity_level",
        ArcLogVerbosityV::instance()
    ));
    // use ansi?
    bool use_ansi_b = *g_metadata->get("outputs.StdOutput.use_ansi", ME_BOOLV);
    arclog::StdOutput::UseANSI use_ansi = arclog::StdOutput::USEANSI_NEVER;
    if(use_ansi_b)
    {
        use_ansi = arclog::StdOutput::USEANSI_ALWAYS;
    }
    std_output->set_use_ansi(use_ansi);
    // add to handler
    omi::report::log_handler.add_output(std_output);
}

static void init_file_output()
{
    // build the path to write logs to
    arc::io::sys::Path log_path =
        *g_metadata->get("outputs.FileOutput.base_path", ME_PATHV);

    // get time
    std::chrono::time_point<std::chrono::system_clock> now =
        std::chrono::system_clock::now();
    time_t now_t = std::chrono::system_clock::to_time_t(now);
    // get the syntax to read the date as
    arc::str::UTF8String date_syntax =
        *g_metadata->get("outputs.FileOutput.path_date_syntax", ME_U8STRV);
    // get the date
    char date_buffer[50];
    strftime(
        date_buffer,
        sizeof(date_buffer),
        date_syntax.get_raw(),
        std::localtime(&now_t)
    );
    log_path << date_buffer;
    // get the syntax to read the time as
    arc::str::UTF8String time_syntax =
        *g_metadata->get("outputs.FileOutput.path_time_syntax", ME_U8STRV);
    // get the time
    char time_buffer[50];
    strftime(
        time_buffer,
        sizeof(time_buffer),
        time_syntax.get_raw(),
        std::localtime(&now_t)
    );
    arc::str::UTF8String file_name(time_buffer);
    // get the file extension
    arc::str::UTF8String file_extension =
        *g_metadata->get("outputs.FileOutput.file_extension", ME_U8STRV);
    file_name << "." << file_extension;
    // update the path
    log_path << file_name;

    // create the file output
    file_output = new arclog::FileOutput(log_path, false);

    // enabled?
    bool enabled = *g_metadata->get("outputs.FileOutput.enabled", ME_BOOLV);
    // need to test this since enabling the file writer opens it
    try
    {
        file_output->set_enabled(enabled);
    }
    catch(const arc::ex::ArcException& exc)
    {
        // report the error using std::cerr since logging is not setup yet
        std::cerr << "Failed to open log file output to: \"" << log_path
                  << "\". With error: \"" << exc.what() << "\". Logging to "
                  << "file will be disabled." << std::endl;
        return;
    }

    // verbosity
    file_output->set_verbosity_level(*g_metadata->get(
        "outputs.FileOutput.verbosity_level",
        ArcLogVerbosityV::instance()
    ));
    // add to handler
    omi::report::log_handler.add_output(file_output);
}

//------------------------------------------------------------------------------
//                             LOG VERBOSITY VISITOR
//------------------------------------------------------------------------------

ArcLogVerbosityV& ArcLogVerbosityV::instance()
{
    static ArcLogVerbosityV v;
    return v;
}

bool ArcLogVerbosityV::retrieve(
        const Json::Value* data,
        const arc::str::UTF8String& key,
        metaengine::Document* requester,
        arc::str::UTF8String& error_message)
{
    // check type
    if(data->isString())
    {
        arc::str::UTF8String str(data->asCString());
        if(str == "critical")
        {
            m_value = arclog::VERBOSITY_CRITICAL;
        }
        else if(str == "error")
        {
            m_value = arclog::VERBOSITY_ERROR;
        }
        else if(str == "warning")
        {
            m_value = arclog::VERBOSITY_WARNING;
        }
        else if(str == "notice")
        {
            m_value = arclog::VERBOSITY_NOTICE;
        }
        else if(str == "info")
        {
            m_value = arclog::VERBOSITY_INFO;
        }
        else if(str == "debug")
        {
            m_value = arclog::VERBOSITY_DEBUG;
        }
        else
        {
            error_message << "\"" << str << "\" cannot be converted to "
                          << "arclog::Verbosity since it is not a recognised "
                          << "verbosity level.";
            return false;
        }
    }
    else
    {
        error_message << "\"" << data->toStyledString() << "\" cannot be "
                      << "converted to arclog::Verbosity since it is not of "
                      << "string type.";
        return false;
    }

    return true;
}

} // namespace report
} // namespace omi
