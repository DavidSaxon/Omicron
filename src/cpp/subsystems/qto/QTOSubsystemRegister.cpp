#ifndef OMICRON_SUBSYSTEMS_QTO_SUBSYSTEM_REGISTER_HPP_
#define OMICRON_SUBSYSTEMS_QTO_SUBSYSTEM_REGISTER_HPP_

#include <cstring>

#include <arcanecore/base/str/UTF8String.hpp>
#include <arcanecore/io/dl/DLOperations.hpp>

#include <omicron/subsystem/SubsystemCommon.h>

// TODO: change the name of this file


// TODO: REMOVE ME
#include <QtWidgets/QApplication>
#include "subsystems/qto/MainWindow.hpp"

namespace qto
{

extern "C"
{

//------------------------------------------------------------------------------
//                                   FUNCTIONS
//------------------------------------------------------------------------------

/*!
 * \brief Registers this subsystem within the Omicron engine.
 *
 * \param name Returns the subsystem's unique name.
 * \param version Returns the subsystem's version number.
 * \param role Returns returns the roles this subsystem fulfills.
 */
ARC_IO_DL_EXPORT void oss_register_subsystem(
        char** name,
        char** version,
        ossRole& roles)
{
    arc::str::UTF8String temp_name("qto");
    *name = new char[temp_name.get_byte_length()];
    memcpy(*name, temp_name.get_raw(), temp_name.get_byte_length());

    arc::str::UTF8String temp_version("0.0.1");
    *version = new char[temp_version.get_byte_length()];
    memcpy(*version, temp_version.get_raw(), temp_version.get_byte_length());

    roles = static_cast<ossRole>(
        OSS_ROLE_WINDOW_MANAGER |
        OSS_ROLE_INPUT          |
        OSS_ROLE_UI
    );

    // TODO: REMOVE ME
    int argc = 0;
    QApplication app(argc, nullptr);
    MainWindow* window = new MainWindow();
    window->show();
    QApplication::exec();
}

// TODO: register main loop callback

// TODO: set initial settings

// TODO: load stored settings

// TODO: deferred gl context

// TODO: DOC
ARC_IO_DL_EXPORT void oss_wm_deferred_open_main()
{
    // TODO: this might not start the qt application?
}

// TODO:

// TODO: exit application

// TODO: event shit

} // extern "C"

} // namespace qto

#endif
