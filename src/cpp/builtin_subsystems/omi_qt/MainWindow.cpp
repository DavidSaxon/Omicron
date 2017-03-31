#include "omi_qt/MainWindow.hpp"

#include <QtCore/QCoreApplication>
#include <QtCore/QEvent>

#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

// TODO: REMOVE ME
#include <iostream>

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
    if(m_device != nullptr)
    {
        delete m_device;
    }
}

//------------------------------------------------------------------------------
//                            PUBLIC MEMBER FUNCTIONS
//------------------------------------------------------------------------------

void MainWindow::initialize()
{
    // TODO: how to get logger from Omicron
    std::cout << "initialize called." << std::endl;
}

void MainWindow::render(QPainter* painter)
{
    // TODO: how to get logger from Omicron
    std::cout << "render with painter called." << std::endl;

    Q_UNUSED(painter);
}

void MainWindow::render()
{
    // TODO: how to get logger from Omicron
    std::cout << "plain render called" << std::endl;

    if(m_device == nullptr)
    {
        m_device = new QOpenGLPaintDevice();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_device->setSize(size());

    // TODO: does the painter need to be created everytime?
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
        // TODO: how to get logger from Omicron
        std::cout << "can't render because the window is not exposed"
                  << std::endl;
        return;
    }

    // does the context need initialisation?
    bool needs_initialise = false;

    // TODO: move initialization to it's own function
    // create a new context?
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

    // TODO: how to get logger from Omicron
    std::cout << "expose event!" << std::endl;

    // trigger render on expose
    if(isExposed())
    {
        renderNow();
    }
}
