#include "subsystems/qto/MainWindow.hpp"

#include <QtCore/QCoreApplication>
#include <QtCore/QEvent>

#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

// TODO: REMOVE ME
#include <iostream>

namespace qto
{

//------------------------------------------------------------------------------
//                                  CONSTRUCTORS
//------------------------------------------------------------------------------

MainWindow::MainWindow()
    :
    QWindow         (),
    m_context       (nullptr),
    m_device        (nullptr),
    m_update_pending(false),
    m_animating     (false)
{
    setSurfaceType(QWindow::OpenGLSurface);
}

//------------------------------------------------------------------------------
//                                   DESTRUCTOR
//------------------------------------------------------------------------------

MainWindow::~MainWindow()
{
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void MainWindow::initialize()
{
    std::cout << "initialize called." << std::endl;
}

void MainWindow::render(QPainter* painter)
{
    std::cout << "render with painter called." << std::endl;

    Q_UNUSED(painter);
}

void MainWindow::render()
{
    std::cout << "plain render called" << std::endl;

    if(!m_device)
    {
        m_device = new QOpenGLPaintDevice();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_device->setSize(size());

    QPainter painter(m_device);
    render(&painter);
}

//------------------------------------------------------------------------------
//                                  PUBLIC SLOTS
//------------------------------------------------------------------------------

void MainWindow::renderLater()
{
    // if an update is not running/pending, start a new one
    if(!m_update_pending)
    {
        m_update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

void MainWindow::renderNow()
{
    // only render if the window is exposed
    if(!isExposed())
    {
        std::cout << "can't render because the window is not exposed"
                  << std::endl;
        return;
    }

    // does the context need initialisation?
    bool needs_initialise = false;

    // create a new context?
    if(!m_context)
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

    render();

    m_context->swapBuffers(this);

    if(m_animating)
    {
        renderLater();
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
            renderNow();
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

    std::cout << "expose event!" << std::endl;

    // trigger render on expose
    if(isExposed())
    {
        renderNow();
    }
}

} // namespace qto
