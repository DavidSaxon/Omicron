#ifndef OMICRON_WINDOWMANAGEMENT_MAINWINDOW_HPP_
#define OMICRON_WINDOWMANAGEMENT_MAINWINDOW_HPP_

#include <arcanecore/base/Preproc.hpp>

namesapce omi
{
namespace wm
{

/*!
 * \brief Represents the main window for Omicron. This window will create the
 *        GL context for the renderer.
 *
 * The internals of this class is implemented by the WindowManager subsystem.
 */
class MainWindow
{
private:

        ARC_DISALLOW_COPY_AND_ASSIGN(MainWindow);

public:

    //--------------------------------------------------------------------------
    //                          PUBLIC STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns the singleton instance of the main window.
     */
    static MainWindow* get_instance();

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    ~MainWindow();

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Returns whether the main window is currently open.
     */
    bool is_open() const;

    /*!
     * \brief Opens the main window if it is currently closed.
     *
     * \note If the main window is already open, this function will do nothing.
     */
    void open();

    /*!
     * \brief Closes the main window if it is currently open.
     *
     * \note If the main window is already closed, this function will do
     *       nothing.
     */
    void close();

protected:

    //--------------------------------------------------------------------------
    //                         PROTECTED STATIC FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * \brief Performs the startup routine for window management services.
     *
     * \param subsystem_dl The dynamic library of the window management
     *                     subsystem to use.
     */
    static void startup_routine(void* subsystem_dl);

    /*!
     * \brief Performs the shutdown routine for window management services.
     */
    static void shutdown();

private:

    //--------------------------------------------------------------------------
    //                             STATIC ATTRIBUTES
    //--------------------------------------------------------------------------

    /*!
     * \brief Whether window management services have been successfully
     *        initialised yet or not.
     */
    static bool s_initialised;

    //--------------------------------------------------------------------------
    //                            PRIVATE CONSTRUCTOR
    //--------------------------------------------------------------------------

    /*!
     * \brief Constructs a Main window instance.
     */
    MainWindow();
};

} // namespace wm
} // namespace omi

#endif
