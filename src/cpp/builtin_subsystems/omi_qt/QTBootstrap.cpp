#include "omi_qt/QTBootstrap.hpp"

#include <omicron/api/report/Logging.hpp>
#include <omicron/api/window/subsystem/WindowInterface.hpp>

#include <QtWidgets/QApplication>

#include "omi_qt/QTGlobals.hpp"


//------------------------------------------------------------------------------
//                               REGISTER SUBSYSTEM
//------------------------------------------------------------------------------

OMICRON_API_WINDOW_SUBSYSTEM_REGISTER(omi_qt::QTBootstrap)

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

QTBootstrap::~QTBootstrap()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

QTBootstrap* QTBootstrap::get_instance()
{
    static QTBootstrap instance;
    return &instance;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void QTBootstrap::startup()
{
    // set up logging
    global::logger =
        omi::report::log_handler.vend_input(arc::log::Profile("OMICRON-QT"));

    global::logger->info << "Starting Omicron Qt subsystem." << std::endl;

    // initialize Qt with empty arguments
    m_qt_application = new QApplication(g_argc, &g_argv[0]);

    // instantiate the main window
    // TODO:

    // super call
    omi::window::ss::Bootstrap::startup();
}

void QTBootstrap::shutdown()
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

void QTBootstrap::start_main_loop(
        omi::window::ss::EngineCycleFunc* engine_cycle_func)
{
    global::logger->debug << "Entering the main loop." << std::endl;

    // TODO: set main loop on main window


    // TODO: show the main window

    // begin the main loop
    m_qt_application->exec();

    global::logger->debug << "Exiting the main loop." << std::endl;
}

//------------------------------------------------------------------------------
//                              PRIVATE CONSTRUCTORS
//------------------------------------------------------------------------------

QTBootstrap::QTBootstrap()
    : omi::window::ss::Bootstrap()
    , m_qt_application          (nullptr)
{
}

} // namespace omi_qt



