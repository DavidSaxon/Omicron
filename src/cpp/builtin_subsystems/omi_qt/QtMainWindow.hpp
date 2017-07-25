/*!
 * \file
 * \author David Saxon
 */
#ifndef OMI_QT_MAINWINDOW_HPP_
#define OMI_QT_MAINWINDOW_HPP_

#include <omicron/api/window/MainWindow.hpp>
#include <omicron/api/window/subsystem/WindowInterface.hpp>

#include <QtGui/QOpenGLFunctions>
#include <QtGui/QWindow>

//------------------------------------------------------------------------------
//                              FORWARD DECLARATIONS
//------------------------------------------------------------------------------

class QOpenGLPaintDevice;

namespace omi_qt
{

class QtBootstrap;

/*!
 * \brief The Qt implementation of Omicron's main window.
 */
class QtMainWindow
    : public QWindow
    , public omi::window::MainWindow
    , protected QOpenGLFunctions
{
Q_OBJECT

    //--------------------------------------------------------------------------
    //                                  FRIENDS
    //--------------------------------------------------------------------------

    friend class QtBootstrap;

public:

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~QtMainWindow();

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    static QtMainWindow* get_instance();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    // omi::window::MainWindow override
    virtual omi::window::WindowMode get_mode() const;

    // omi::window::MainWindow override
    virtual void set_mode(omi::window::WindowMode mode);

    // QWindow override
    virtual void initialize();

    // QWindow override
    virtual void render(QPainter* painter);

    // QWindow override
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
    //                           PROTECTED CONSTRUCTOR
    //--------------------------------------------------------------------------

    QtMainWindow();

    //--------------------------------------------------------------------------
    //                         PROTECTED MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Sets the function that will be used to update a cycle of the
     *        engine.
     */
    void set_engine_cycle(omi::window::ss::EngineCycleFunc* engine_cycle_func);

    /// QWindow override
    bool event(QEvent* event) Q_DECL_OVERRIDE;

    // QWindow override
    void exposeEvent(QExposeEvent* event) Q_DECL_OVERRIDE;

private:

    /*!
     * \brief THe function that will be used to update a cycle of the engine.
     */
    omi::window::ss::EngineCycleFunc* m_engine_cycle_func;

    /*!
     * \brief The OpenGL context acquired from Qt.
     */
    QOpenGLContext* m_context;
    /*!
     * \brief The Qt paint device for displaying OpenGL.
     */
    QOpenGLPaintDevice* m_device;
    /*!
     * \brief Signifies the the render will be called in the next update cycle.
     */
    bool m_update_pending;
};

} // namespace omi_qt

#endif
