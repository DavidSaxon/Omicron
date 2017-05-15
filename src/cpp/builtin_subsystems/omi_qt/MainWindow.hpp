/*!
 * \file
 * \author David Saxon
 */
#ifndef OMI_QT_MAINWINDOW_HPP_
#define OMI_QT_MAINWINDOW_HPP_

#include <omicron/subsystem/Input.hpp>

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

    /*!
     * \brief Sets the function that will be used to update a cycle of the
     *        engine.
     */
    void set_engine_cycle(omi::ss::Input::EngineCycleFunc engine_cycle);

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
     * \brief Performs a rendering and update cycle of Omicron.
     */
    void render_now();

    /*!
     * \brief Calling this function will cause render_now to be called again on
     *        the next event cycle.
     */
    void render_later();

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
     * \brief The function that will be used to update a cycle of the engine.
     */
    omi::ss::Input::EngineCycleFunc m_engine_cycle;

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
};

#endif
