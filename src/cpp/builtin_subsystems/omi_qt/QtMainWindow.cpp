#include "omi_qt/QtMainWindow.hpp"

#include <QtCore/QCoreApplication>
#include <QtCore/QEvent>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QtWidgets/QApplication>


namespace omi_qt
{

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

QtMainWindow::~QtMainWindow()
{
    if(m_device != nullptr)
    {
        delete m_device;
    }

    if(m_context != nullptr)
    {
        delete m_context;
    }
}

//------------------------------------------------------------------------------
//                            PUBLIC STATIC FUNCTIONS
//------------------------------------------------------------------------------

QtMainWindow* QtMainWindow::get_instance()
{
    static QtMainWindow instance;
    return &instance;
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

omi::window::WindowMode QtMainWindow::get_mode() const
{
    // TODO:
    return omi::window::kModeWindowed;
}

void QtMainWindow::set_mode(omi::window::WindowMode mode)
{
    // TODO:
}

void QtMainWindow::initialize()
{
    // TODO: needed?
}

void QtMainWindow::render(QPainter* painter)
{
    Q_UNUSED(painter);

    // perform a cycle of the engine
    if(!m_engine_cycle_func())
    {
        // execution has ended, stop Qt
        QApplication::exit();
    }
}

void QtMainWindow::render()
{
    if(m_device == nullptr)
    {
        m_device = new QOpenGLPaintDevice();
    }

    m_device->setSize(size());

    // TODO: does the painter need to be created everytime?
    QPainter painter(m_device);
    render(&painter);
}

//------------------------------------------------------------------------------
//                                  PUBLIC SLOTS
//------------------------------------------------------------------------------

void QtMainWindow::render_now()
{
    // only render if the window is exposed
    if(!isExposed())
    {
        return;
    }

    // does the context need initialisation?
    bool needs_initialise = false;

    // create a new context
    if(m_context == nullptr)
    {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();

        needs_initialise = true;
    }

    // make this the current context
    m_context->makeCurrent(this);

    // initialise the context?
    if(needs_initialise)
    {
        initializeOpenGLFunctions();
        initialize();
    }

    // hand off to the render function (which will update the engine)
    render();
    // swap buffers
    m_context->swapBuffers(this);
    // prepare to render again
    render_later();
}

void QtMainWindow::render_later()
{
    // if an update is not running/pending, start a new one
    if(!m_update_pending)
    {
        m_update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

//------------------------------------------------------------------------------
//                             PROTECTED CONSTRUCTOR
//------------------------------------------------------------------------------

QtMainWindow::QtMainWindow()
    : QWindow                ()
    , omi::window::MainWindow()
    , m_engine_cycle_func    (nullptr)
    , m_context              (nullptr)
    , m_device               (nullptr)
    , m_update_pending       (false)
{
    setSurfaceType(QWindow::OpenGLSurface);
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void QtMainWindow::set_engine_cycle(
        omi::window::ss::EngineCycleFunc* engine_cycle_func)
{
    m_engine_cycle_func = engine_cycle_func;
}

bool QtMainWindow::event(QEvent* event)
{
    switch(event->type())
    {
        case QEvent::UpdateRequest:
        {
            // render!
            m_update_pending = false;
            render_now();
            return true;
        }
        default:
        {
            // super call
            return QWindow::event(event);
        }
    }
}

void QtMainWindow::exposeEvent(QExposeEvent* event)
{
    Q_UNUSED(event);

    // trigger render on expose
    if(isExposed())
    {
        render_now();
    }
}

} // namespace omi_qt
