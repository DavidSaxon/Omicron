/*!
 * \file
 * \author David Saxon
 * \date 2017
 * \copyright Copyright (C) The Arcane Initiative - All Rights Reserved
 *            Unauthorized copying of this file, via any medium is strictly
 *            prohibited
 */
#include "deathray/impl/Globals.hpp"

#include <arcanecore/base/Preproc.hpp>

#ifdef DEATH_LOGGING
    #include <arcanecore/log/outputs/StdOutput.hpp>
#endif


namespace death
{
namespace global
{

//------------------------------------------------------------------------------
//                                   VARIABLES
//------------------------------------------------------------------------------

#ifdef DEATH_LOGGING

    arc::log::LogHandler log_handler;
    arc::log::Input* logger = nullptr;

    static arc::log::StdOutput* std_output = nullptr;

#endif

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

void setup_logging()
{
    #ifdef DEATH_LOGGING

        // bail out early
        if(logger != nullptr)
        {
            return;
        }

        // TODO: determine how to control these settings

        // generate std out output
        std_output = new arc::log::StdOutput();
        std_output->set_enabled(true);
        std_output->set_verbosity_level(arc::log::VERBOSITY_DEBUG);
        std_output->set_use_ansi(arc::log::StdOutput::USEANSI_IF_SUPPORTED);
        log_handler.add_output(std_output);

        // vend the input
        logger = log_handler.vend_input(arc::log::Profile("DEATHRAY"));

    #endif
}

} // namespace global
} // namespace death
