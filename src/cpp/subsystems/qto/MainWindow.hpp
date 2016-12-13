/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_SUBSYSTEMS_QTO_MAINWINDOW_HPP_
#define OMICRON_SUBSYSTEMS_QTO_MAINWINDOW_HPP_

#include <QtGui/QOpenGLFunctions>
#include <QtGui/QWindow>

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class QOpenGLPaintDevice;

namespace qto
{

class MainWindow : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT

public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    MainWindow();

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    ~MainWindow();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    virtual void initialize();

    virtual void render(QPainter* painter);

    virtual void render();

public slots:

    //--------------------------------------------------------------------------
    //                                PUBLIC SLOTS
    //--------------------------------------------------------------------------

    void renderLater();

    void renderNow();

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    bool event(QEvent* event) Q_DECL_OVERRIDE;

    void exposeEvent(QExposeEvent* event) Q_DECL_OVERRIDE;

private:

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    QOpenGLContext* m_context;
    QOpenGLPaintDevice* m_device;

    bool m_update_pending;
    bool m_animating;
};

} // namespace qto

#endif
