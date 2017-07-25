#include "omi_qt/QtBootstrap.hpp"

#include <omicron/api/report/Logging.hpp>
#include <omicron/api/window/subsystem/WindowInterface.hpp>

#include <QtWidgets/QApplication>

#include "omi_qt/QtGlobals.hpp"
#include "omi_qt/QtMainWindow.hpp"


//------------------------------------------------------------------------------
//                               REGISTER SUBSYSTEM
//------------------------------------------------------------------------------

OMICRON_API_WINDOW_SUBSYSTEM_REGISTER(
    omi_qt::QtBootstrap,
    omi_qt::QtMainWindow
);

namespace omi_qt
{

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
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

QtBootstrap::~QtBootstrap()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

QtBootstrap* QtBootstrap::get_instance()
{
    static QtBootstrap instance;
    return &instance;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void QtBootstrap::startup()
{
    // set up logging
    global::logger =
        omi::report::log_handler.vend_input(arc::log::Profile("OMICRON-QT"));

    global::logger->info << "Starting Omicron Qt subsystem." << std::endl;

    // initialize Qt with empty arguments
    m_qt_application = new QApplication(g_argc, &g_argv[0]);

    // instantiate the main window
    QtMainWindow::get_instance();

    // super call
    omi::window::ss::Bootstrap::startup();
}

void QtBootstrap::shutdown()
{
    global::logger->info << "Shutting down Omicron Qt subsystem." << std::endl;

    // destroy the Qt application
    delete m_qt_application;
    m_qt_application = nullptr;

    // remove the logger (NOTE: this shouldn't need to be done, but on Windows:
    // closing this DLL causes the memory for the logging input to be freed,
    // even though this DLL doesn't own it).
    omi::report::log_handler.remove_input(global::logger);

    // super call
    omi::window::ss::Bootstrap::shutdown();
}

void QtBootstrap::start_main_loop(
        omi::window::ss::EngineCycleFunc* engine_cycle_func)
{
    global::logger->debug << "Entering the main loop." << std::endl;

    // set main loop on main window
    QtMainWindow::get_instance()->set_engine_cycle(engine_cycle_func);
    // show the main window
    QtMainWindow::get_instance()->show();

    // begin the main loop
    m_qt_application->exec();

    global::logger->debug << "Exiting the main loop." << std::endl;
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTORS
//------------------------------------------------------------------------------

QtBootstrap::QtBootstrap()
    : omi::window::ss::Bootstrap()
    , m_qt_application          (nullptr)
{
}

} // namespace omi_qt



