/*!
 * \file
 * \author David Saxon
 */
#ifndef OSS_WINDOWMANAGER_HPP_
#define OSS_WINDOWMANAGER_HPP_

#include "oss/Subsystem.hpp"

namespace oss
{

/*!
 * \brief TODO:
 */
class WindowManager : virtual public Subsystem
{
public:

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
     * \brief TODO
     */
    virtual void open_main_window() = 0;
};

} // namespace oss

#endif
