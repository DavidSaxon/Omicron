#include "omi_qt/MainWindow.hpp"

#include <QtCore/QCoreApplication>
#include <QtCore/QEvent>
#include <QtWidgets/QApplication>

#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

MainWindow::MainWindow()
    : QWindow         ()
    , m_context       (nullptr)
    , m_device        (nullptr)
    , m_update_pending(false)
{
    setSurfaceType(QWindow::OpenGLSurface);
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

MainWindow::~MainWindow()
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
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void MainWindow::set_engine_cycle(omi::ss::Input::EngineCycleFunc engine_cycle)
{
    m_engine_cycle = engine_cycle;
}

void MainWindow::initialize()
{
    // TODO: needed?
}

void MainWindow::render(QPainter* painter)
{
    Q_UNUSED(painter);

    // perform a cycle of the engine
    if(!m_engine_cycle())
    {
        // execution has ended, stop Qt
        QApplication::exit();
    }
}

void MainWindow::render()
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

void MainWindow::render_now()
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

void MainWindow::render_later()
{
    // if an update is not running/pending, start a new one
    if(!m_update_pending)
    {
        m_update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

//------------------------------------------------------------------------------
//                           PROTECTED MEMBER FUNCTIONS
//------------------------------------------------------------------------------

bool MainWindow::event(QEvent* event)
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

void MainWindow::exposeEvent(QExposeEvent* event)
{
    Q_UNUSED(event);

    // trigger render on expose
    if(isExposed())
    {
        render_now();
    }
}
