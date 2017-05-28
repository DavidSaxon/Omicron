/*!
 * \file
 * \author David Saxon
 */
#ifndef OMICRON_SUBSYSTEM_WINDOWMANAGER_HPP_
#define OMICRON_SUBSYSTEM_WINDOWMANAGER_HPP_

#include <omicron/api/window/MainWindow.hpp>

#include "omicron/subsystem/Subsystem.hpp"


namespace omi
{
namespace ss
{

/*!
 * \brief TODO:
 */
class WindowManager : virtual public Subsystem
{
public:

    //--------------------------------------------------------------------------
    //                                ENUMERATORS
    //--------------------------------------------------------------------------

    /*!
     * \brief Defines the different modes a window can be in.
     */
    enum WindowMode
    {
        /*!
         * \brief The window is a standard window with borders.
         */
        kModeWindowed = 0,
        /*!
         * \brief A window without standard operating system provided borders.
         */
        kModeBorderless,
        /*!
         * \brief A window without borders and also occupies the entire screen.
         */
        kModeFullscreen
    };

    //--------------------------------------------------------------------------
    //                                CONSTRUCTORS
    //--------------------------------------------------------------------------

    /*!
     * \brief TODO:
     */
    WindowManager()
        : Subsystem()
    {
        // set window manager role
        m_roles = static_cast<Role>(m_roles | kRoleWindowManager);
    }

    //--------------------------------------------------------------------------
    //                                 DESTRUCTOR
    //--------------------------------------------------------------------------

    virtual ~WindowManager()
    {
    }

    //--------------------------------------------------------------------------
    //                          PUBLIC MEMBER FUNCTIONS
    //--------------------------------------------------------------------------

    /*!
     * Sets the window mode to be used.
     */
    virtual void set_mode(WindowMode mode) = 0;
};

} // namespace ss
} // namespace omi

#endif
