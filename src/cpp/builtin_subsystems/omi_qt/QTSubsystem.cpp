#include "omi_qt/QTSubsystem.hpp"

#include <QtWidgets/QApplication>

#include <omicron/report/Logging.hpp>

#include "omi_qt/MainWindow.hpp"
#include "omi_qt/QTGlobals.hpp"

//------------------------------------------------------------------------------
//                               REGISTER SUBSYSTEM
//------------------------------------------------------------------------------

OSS_REGISTER_SUBSYSTEM(QTSubsystem);

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

QTSubsystem::QTSubsystem()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void QTSubsystem::boot()
{
    // set up logging
    global::logger =
        omi::report::log_handler.vend_input(arclog::Profile("OMICRON-QT"));

    global::logger->info << "Starting up subsystem." << std::endl;

    // initialize Qt with empty arguments
    int argc = 0;
    QApplication app(argc, nullptr);
}

void QTSubsystem::shutdown()
{
    global::logger->info << "Shutting down subsystem." << std::endl;

    // remove the logger (NOTE: this shouldn't need to be done, but on Windows:
    // closing this DLL causes the memory for the logging input to be freed,
    // even though this DLL doesn't own it).
    omi::report::log_handler.remove_input(global::logger);
}

// TODO: init settings

void QTSubsystem::open_main_window()
{
    // TODO: how to get logging from omicron
    // std::cout << "Omicron QT open main window" << std::endl;

    // create the main window and show it
    MainWindow* window = new MainWindow();
    window->show();

    // TODO: move this some where
    QApplication::exec();
}
