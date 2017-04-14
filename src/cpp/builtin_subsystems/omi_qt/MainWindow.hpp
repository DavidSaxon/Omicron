/*!
 * \file
 * \author David Saxon
 */
#ifndef OMI_QT_MAINWINDOW_HPP_
#define OMI_QT_MAINWINDOW_HPP_

#include <QtGui/QOpenGLFunctions>
#include <QtGui/QWindow>

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class QOpenGLPaintDevice;

/*!
 * \brief TODO
 */
class MainWindow : public QWindow, protected QOpenGLFunctions
{
Q_OBJECT
public:

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     */
    MainWindow();

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~MainWindow();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // TODO: qt inherit
    virtual void initialize();

    // TODO: qt inherit
    virtual void render(QPainter* painter);

    // TODO: qt inherit
    virtual void render();

public slots:

    //--------------------------------------------------------------------------
    //                                PUBLIC SLOTS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     */
    void renderLater();

    /*!
     * \brief TODO
     */
    void renderNow();

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     */
    bool event(QEvent* event) Q_DECL_OVERRIDE;

    /*!
     * \brief TODO
     */
    void exposeEvent(QExposeEvent* event) Q_DECL_OVERRIDE;

private:

    //--------------------------------------------------------------------------
    //                          PRIVATE MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO
     */
    QOpenGLContext* m_context;
    /*!
     * \brief TODO
     */
    QOpenGLPaintDevice* m_device;

    /*!
     * \brief TODO
     */
    bool m_update_pending;
    /*!
     * \brief TODO
     */
    bool m_animating;
};

#endif