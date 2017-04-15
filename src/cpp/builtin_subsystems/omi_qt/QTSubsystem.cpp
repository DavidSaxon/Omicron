#include "omi_qt/QTSubsystem.hpp"

#include <cassert>

#include <QtWidgets/QApplication>

#include <omicron/report/Logging.hpp>

#include "omi_qt/QTGlobals.hpp"

//------------------------------------------------------------------------------
//                               REGISTER SUBSYSTEM
//------------------------------------------------------------------------------

OSS_REGISTER_SUBSYSTEM(QTSubsystem);

//------------------------------------------------------------------------------
//                                    GLOBALS
//------------------------------------------------------------------------------

namespace
{
// for Qt
static int g_argc = 1;
static std::vector<char*> g_argv = {const_cast<char*>("omicron")};
} // namespace  anonymous

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

QTSubsystem::QTSubsystem()
    : m_qt_application(nullptr)
    , m_main_window   (nullptr)
    , m_mode          (omi::ss::WindowManager::kModeWindowed)
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void QTSubsystem::startup()
{
    // set up logging
    global::logger =
        omi::report::log_handler.vend_input(arclog::Profile("OMICRON-QT"));

    global::logger->info << "Starting up subsystem." << std::endl;

    // initialize Qt with empty arguments
    m_qt_application = new QApplication(g_argc, &g_argv[0]);

    // create the main window instance
    m_main_window = new MainWindow();
}

void QTSubsystem::shutdown()
{
    global::logger->info << "Shutting down subsystem." << std::endl;

    // destroy the main window
    delete m_main_window;
    m_main_window = nullptr;

    // destroy the Qt application
    delete m_qt_application;
    m_qt_application = nullptr;

    // remove the logger (NOTE: this shouldn't need to be done, but on Windows:
    // closing this DLL causes the memory for the logging input to be freed,
    // even though this DLL doesn't own it).
    omi::report::log_handler.remove_input(global::logger);
}

void QTSubsystem::set_mode(WindowMode mode)
{
    assert(m_main_window != nullptr);

    m_mode = mode;

    switch(m_mode)
    {
        case omi::ss::WindowManager::kModeWindowed:
        {
            m_main_window->setWindowState(Qt::WindowNoState);
            m_main_window->setFlags(Qt::Window);
            break;
        }
        case omi::ss::WindowManager::kModeBorderless:
        {
            m_main_window->setWindowState(Qt::WindowNoState);
            m_main_window->setFlags(Qt::FramelessWindowHint);
            break;
        }
        case omi::ss::WindowManager::kModeFullscreen:
        {
            m_main_window->setWindowState(Qt::WindowFullScreen);
            m_main_window->setFlags(Qt::Window);
            break;
        }
    }
}

void QTSubsystem::start_main_loop(omi::ss::Input::EngineCycleFunc engine_cycle)
{
    global::logger->debug << "Entering the main loop." << std::endl;

    // pass the engine cycle function to the main window.
    // TODO:

    // show the main window
    m_main_window->show();

    // reapply the window mode to make sure it has an effect
    set_mode(m_mode);

    // begin the main loop
    m_qt_application->exec();

    global::logger->debug << "Exiting the main loop." << std::endl;
}

void QTSubsystem::open_main_window()
{
}
