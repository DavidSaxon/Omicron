/*!
 * \file
 * \author David Saxon
 * \brief Logging for Omicron using ArcaneLog.
 */
#ifndef OMICRON_PRIVATE_LOGGING_HPP_
#define OMICRON_PRIVATE_LOGGING_HPP_

#include <arcanelog/Verbosity.hpp>

#include "omicron/private/util/MetaUtil.hpp"

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

namespace arclog
{

class FileOutput;
class Input;
class StdOutput;

} // namespace arclog

namespace omi
{

//------------------------------------------------------------------------------
//                                    GLOBALS
//------------------------------------------------------------------------------

// special case that lives outside of the logging namespace for convenience
/*!
 * \brief The logging input to be used by the Omicron core.
 */
extern arclog::Input* logger;

/*!
 * \brief Logging for Omicron using ArcaneLog.
 */
namespace logging
{

/*!
 * \brief The logging output to std::cout and std::cerr.
 */
extern arclog::StdOutput* std_output;
/*!
 * \brief The logging output for writing to the file system.
 */
extern arclog::FileOutput* file_output;

/*!
 * \brief The MetaEngine Variant for logging configuration.
 */
extern util::meta::VariantPtr metadata;

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief Initialises logging from MetaEngine data.
 */
void init_routine();

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

} // namespace logging
} // namespace omi

#endif
