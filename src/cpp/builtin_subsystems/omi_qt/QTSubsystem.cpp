#include "omi_qt/QTSubsystem.hpp"

#include <arcanelog/Shared.hpp>

#include <QtWidgets/QApplication>

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
    // setup the logger
    if(global::input == nullptr)
    {
        global::input =
            arclog::shared_handler.vend_input(arclog::Profile("OMICRON-QT"));
    }
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void QTSubsystem::boot()
{
    logger->info << "Omicron QT booting." << std::endl;

    // initialize Qt with empty arguments
    int argc = 0;
    QApplication app(argc, nullptr);
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
